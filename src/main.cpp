#include <fcntl.h>           /* For O_* constants */
#include <sys/wait.h>           /* For O_* constants */
#include <semaphore.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mman.h>

#include <fstream>
#include <sys/sem.h>
#include <cstring>
#include <unistd.h>
#include "ConfigParser.h"

using namespace std;

#define MAX_OCUPACAO 9
#define OCUPACAO_OK 8
#define NUMERO_FRAMES 10
// int + long = 12 bytes
#define BYTES_PER_ENTRY 12
#define INT_SIZE 4
#define LONG_SIZE 8

int shared_table;
sem_t *table_sem;
string shd_mem_name;
string table_sem_name;
bool tableCreated, semaphoreCreated, tableMapped;
void *tablePtr;
int status = 0;

size_t sharedMemorySize = NUMERO_FRAMES * BYTES_PER_ENTRY;

void referencia_pagina(int i) {

}

void writePagToTable(void *tablePtr, int frameIdx, int pag) {
    *((int *) (((char *) tablePtr) + frameIdx * BYTES_PER_ENTRY)) = pag;

}

void writeTimeStampToTable(void *tablePtr, int frameIdx, long timestamp) {
    *((long *) (((char *) tablePtr) + frameIdx * BYTES_PER_ENTRY + INT_SIZE)) = timestamp;

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
    table_sem = sem_open(table_sem_name.c_str(), O_CREAT , 0644, 1);


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
        writePagToTable(tablePtr, i, -1);
        writeTimeStampToTable(tablePtr, i, 0);
    }
    sem_post(table_sem);

    string filename = "pag_processo_" + to_string(idx) + ".txt";
    ifstream is(filename.c_str());
    int pag;

    if (!is.is_open()){
        cerr << "could not open file " << filename << endl;
        clean_process_data();
        exit(0);
    }

    while (true && is.is_open()) {
        if (is.eof())
            break;
        is >> pag;
        //ignode comma
        is.ignore();
        cout << pag << endl;
        referencia_pagina(pag);
    }

    clean_process_data();
    exit(0);


}

int main(int argc, char **argv) {
    semaphoreCreated = tableCreated = tableMapped = false;
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
        wait(&status);
    }
    cout << "[main]" << "done" << endl;


}