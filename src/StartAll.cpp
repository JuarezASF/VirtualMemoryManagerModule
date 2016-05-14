//
// Created by jasf on 5/14/16.
//

#include <ios>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "ConfigParser.h"

int PAPQueueSemaphoreID = -1;

void validateSemaphoreKey(int sem_key) {
    if (sem_key < 0) {
        cerr << "Invalid sempaphore key " << sem_key << endl;
        exit(0);
    }

}


int main(int argc, char **argv) {

    ConfigParser::loadConfig("config.txt");

    int sem_key = ConfigParser::getInt("PAPQueueSemaphorKey");

    validateSemaphoreKey(sem_key);

    //check
    cout << "Creating semaphore on key " << sem_key << endl;

    PAPQueueSemaphoreID = semget(sem_key, 2, IPC_CREAT | 0x1FF);
    if (PAPQueueSemaphoreID >= 0)
        cout << "created sem! key: " << sem_key  << " id:  " << PAPQueueSemaphoreID << endl;
    else {
        cerr << "Error creating semaphore!" << endl;
        cerr << strerror(errno) << endl;
    }


}

