#include <signal.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <semaphore.h>
#include <sys/wait.h>           /* For O_* constants */
#include <sys/mman.h>

#include <fstream>
#include <cstring>
#include <ctime>

#include "ConfigParser.h"

using namespace std;

#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8
#define NUMERO_FRAMES 10
#define INT_SIZE 4
#define LONG_SIZE 4
#define MAX_LONG 0x7FFFFFFFFFFFFFFFL
#define MAX_INT 0x7FFFFFFF

bool quitRequested = false;

void foo(int d) {

}

typedef struct TableEntry {
    int page;
    long timestamp;
    bool occupied;
    int nextFree;
} TableEntry;

typedef struct PageFrameTable {
    int qtdFree;
    int pageFaultCount;
    TableEntry table[NUMERO_FRAMES];
} PageFrameTable;


int shared_table;
sem_t *table_sem;
string shd_mem_name;
//semaphore to synchronize acess to table
string table_sem_name;

// table is mapped here
void *tablePtr;

//used to communicate between server and user process
int childRequestPipe[2];
int serverResponsePipe[2];

//used to send signal between allocator and substituter
int pageAllocationProcessId;
int pageSubstitutionProcessId;

size_t sharedMemorySize = sizeof(PageFrameTable);

bool pipeCreated, semaphoreCreated, tableCreated, tableMapped;

char childRequestBuffer[256];

int referencia_pagina(int i) {
    ((int *) childRequestBuffer)[0] = i;
    write(childRequestPipe[1], childRequestBuffer, INT_SIZE);
    read(serverResponsePipe[0], childRequestBuffer, INT_SIZE);
    return ((int *)childRequestBuffer)[0];

}

void writePagToTable(int frameIdx, int pag) {
    ((PageFrameTable *) tablePtr)->table[frameIdx].page = pag;
}

void writeTimeStampToTable(int frameIdx, long timestamp) {
    ((PageFrameTable *) tablePtr)->table[frameIdx].timestamp = timestamp;
}

void markEntryAsUsed(int frameIdx) {
    if (!((PageFrameTable *) tablePtr)->table[frameIdx].occupied) {
        ((PageFrameTable *) tablePtr)->qtdFree--;
        ((PageFrameTable *) tablePtr)->table[frameIdx].occupied = true;

    }
}

void markEntryAsFree(int frameIdx) {
    if (((PageFrameTable *) tablePtr)->table[frameIdx].occupied) {
        ((PageFrameTable *) tablePtr)->qtdFree++;
        ((PageFrameTable *) tablePtr)->table[frameIdx].occupied = false;

    }
}

TableEntry readEntry(int frameIdx) {
    return ((PageFrameTable *) tablePtr)->table[frameIdx];
}

/***
 * sequentially reads table and return first available frame.
 *
 * Return -1 if it cannot find available frame
 */
int getAvailableFrame() {

    if (((PageFrameTable *) tablePtr)->qtdFree <= 0)
        return -1;

    for (int i = 0; i < NUMERO_FRAMES; i++) {
        TableEntry e = readEntry(i);
        if (!e.occupied)
            return i;
    }

    return -1;
}

/***
 * sequentially reads table and return frame idx of page
 *
 * Return -1 if it cannot find a frame for that page
 */
int getFrameForPage(int page) {

    if (((PageFrameTable *) tablePtr)->qtdFree <= NUMERO_FRAMES)
        return -1;

    for (int i = 0; i < NUMERO_FRAMES; i++) {
        TableEntry e = readEntry(i);
        if (e.occupied && (e.page == page))
            return i;
    }

    return -1;
}

void pageFaulCountIncr(){
    ((PageFrameTable *)tablePtr)->pageFaultCount++;
}

void clean_process_data() {

    cout << "[pid:" << getpid() << "] dealocating resouces" << endl;
    if (semaphoreCreated) {
        cout << "[pid:" << getpid() << "] dealocating semaphore" << endl;
        sem_close(table_sem);
        sem_unlink(table_sem_name.c_str());
    }
    if (tableMapped) {
        cout << "[pid:" << getpid() << "] unpaming table" << endl;
        munmap(tablePtr, sharedMemorySize);
    }
    if (tableCreated) {
        if (shm_unlink(shd_mem_name.c_str()) < 0) {
            cout << "[pid:" << getpid() << "] deleating table" << endl;
            cout << "ERROR WHILE DEALOCATING SHARED MEMORY" << endl;
            cerr << strerror(errno) << endl;
            exit(0);
        }
        if (close(shared_table) < 0) {
            cout << "[pid:" << getpid() << "] deleating table file descriptor" << endl;
            cout << "ERROR WHILE CLOSING SHARED MEMORY FILE DESCRIPTOR" << endl;
            cerr << strerror(errno) << endl;
            exit(0);
        }

    }
    flush(cout);
    flush(cerr);
}

void replaceOlderPage() {
    int minIdx = -1;
    long minTimeStamp = MAX_LONG;

    //find eldest allocated entry
    for (int i = 0; i < NUMERO_FRAMES; i++) {
        TableEntry e = readEntry(i);

        if (e.occupied && e.timestamp < minTimeStamp) {
            minTimeStamp = e.timestamp;
            minIdx = i;
        }

    }

    if (minIdx >= 0) {
        cout << "pid:" << getpid() << ":: removing entry " << minIdx << endl;
        writeTimeStampToTable(minIdx, MAX_LONG);
        markEntryAsFree(minIdx);

    } else {
        cerr << "could not find entry to deallocate! This is a bug!" << endl;
    }
}


void substitutePages(int d) {
    sem_wait(table_sem);
    while (((PageFrameTable *) tablePtr)->qtdFree <= OCUPACAO_OK) {
        replaceOlderPage();
    }
    sem_post(table_sem);

    kill(d, SIGUSR1);
}

void requestQuit(int d) {
    quitRequested = true;
}

void execute_child(int idx) {
    //create shared memory for table
    shd_mem_name = "/table" + std::to_string(idx);

    cout << "[pid:" << getpid() << "] alocating table" << endl;
//    shared_table = shm_open(shd_mem_name.c_str(), O_CREAT | O_EXCL | O_RDWR, 0644);
    shared_table = shm_open(shd_mem_name.c_str(), O_CREAT | O_RDWR, 0644);

    if (shared_table < 0) {
        cerr << "Error on creating table for process pid:" << idx << endl;
        cerr << strerror(errno) << endl;
        clean_process_data();
        exit(0);
    }
    tableCreated = true;

    //an int for pag idx and a long for timestamp for each frame
    ftruncate(shared_table, sharedMemorySize);

    //map shared memory to local memory
    tablePtr = mmap(NULL, sharedMemorySize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (tablePtr == MAP_FAILED) {
        cerr << "FAILED TO MAP SHARED MEMORY TO CHILD " << to_string(idx) << endl;
        cerr << strerror(errno) << endl;
        clean_process_data();
        exit(0);

    }
    tableMapped = true;

    cout << "[pid:" << getpid() << "] alocating semaphore" << endl;
    table_sem_name = "/sem" + std::to_string(idx);

    //creates semaphore to synchronize access to table
    table_sem = sem_open(table_sem_name.c_str(), O_CREAT, 0644, 1);

    if (table_sem == SEM_FAILED) {
        cerr << "FAILED TO CREATE SEMAPHORE TO CHILD" << to_string(idx) << endl;
        cerr << strerror(errno) << endl;
        clean_process_data();
        exit(0);
    }
    semaphoreCreated = true;

    //zero out table
    sem_wait(table_sem);
    for (int i = 0; i < NUMERO_FRAMES; i++) {
        writePagToTable(i, -1);
        writeTimeStampToTable(i, 0);
        markEntryAsFree(i);
    }
    sem_post(table_sem);

    //create pipe that allows communication between process and server

    if (pipe(childRequestPipe) < 0) {
        cerr << "ERROR WHILE CREATING CHILD REQUEST PIPE!" << endl;
        cerr << strerror(errno) << endl;
        clean_process_data();
        exit(0);

    } else {

    }
    if (pipe(serverResponsePipe) < 0) {
        cerr << "ERROR WHILE CREATING SERVER RESPONSE PIPE!" << endl;
        cerr << strerror(errno) << endl;
        clean_process_data();
        exit(0);

    } else {

    }


    int cpid;
    //start page substitution server process
    if ((cpid = fork()) == 0) {
        //page allocator server
        pageSubstitutionProcessId = getpid();
        signal(SIGUSR2, requestQuit);
        while (!quitRequested) {
            signal(SIGUSR1, substitutePages);
            pause();
        }

        exit(0);

    }
    else {
        pageSubstitutionProcessId = cpid;

    }


    //start process for page allocation
    if ((cpid = fork()) == 0) {
        close(childRequestPipe[0]);
        close(serverResponsePipe[1]);
        pageAllocationProcessId = getpid();

    }
    else {
        close(childRequestPipe[1]);
        close(serverResponsePipe[0]);
        pageAllocationProcessId = cpid;
        char buffer[256];


        while (read(childRequestPipe[1], buffer, 4) > 0) {
            int page = ((int *) buffer)[0];
            int out = getFrameForPage(page);
            if (out < 0) {
                while (out < 0) {
                    pageFaulCountIncr();
                    cout << "page fault for page:"  << page << endl;
                    kill(pageSubstitutionProcessId, SIGUSR1);
                    signal(SIGUSR1, foo);
                    pause();
                    out = getAvailableFrame();
                }
            }
            //once we get here we have a valid frame

            sem_wait(table_sem);
            writeTimeStampToTable(out, (long) time(NULL));
            markEntryAsUsed(out);
            sem_post(table_sem);

            //send answer to client
            ((int *) buffer)[0] = out;
            write(serverResponsePipe[1], buffer, INT_SIZE);


        }


    }


    //open file where we'll read input from
    string filename = "pag_processo_" + to_string(idx) + ".txt";
    ifstream is(filename.c_str());
    if (!is.is_open()) {
        cerr << "could not open file " << filename << endl;
        clean_process_data();
        exit(0);
    }

    //actual child process code
    int pag;
    while (true && is.is_open()) {
        if (is.eof())
            break;
        is >> pag;
        //ignode comma
        is.ignore();
        int frame = referencia_pagina(pag);
    }

    clean_process_data();
    exit(0);


}

int main(int argc, char **argv) {
    pipeCreated = semaphoreCreated = tableCreated = tableMapped = false;
    //parse qtd of process from command line
    if (argc < 2) {
        cerr << "Not enough parameters!" << endl;
        cout << "usage: ./main qtdProcess" << endl;
    }

    int qtdProcess = atoi(argv[1]);
    int child_id;

    for (int i = 0; i < qtdProcess; i++) {
        if ((child_id = fork()) == 0) {
            //child process i
            execute_child(i);
            exit(0);
        } else {
            cout << "[" << "main" << "]" << "started process:#" << i << " pid:" << child_id << endl;
        }
    }

    cout << "[main]" << "waiting for childs" << endl;
    for (int i = 0; i < qtdProcess; i++) {
        int status;
        wait(&status);
    }
    cout << "[main]" << "done" << endl;


}