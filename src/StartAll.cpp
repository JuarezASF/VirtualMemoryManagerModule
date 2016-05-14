//
// Created by jasf on 5/14/16.
//

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include <ios>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "ConfigParser.h"


void validateSemaphoreKey(string sem_key) {
    if (sem_key[0] !=  '/') {
        cerr << "Invalid sempaphore key " << sem_key << endl;
        exit(0);
    }

}

sem_t *queue_sem;

int main(int argc, char **argv) {

    ConfigParser::loadConfig("config.txt");

    string sem_key = ConfigParser::getString("PAPQueueSemaphorKey");

    validateSemaphoreKey(sem_key);

    //check
    cout << "Creating semaphore on key " << sem_key << endl;

    queue_sem =  sem_open(sem_key.c_str(), O_CREAT | O_EXCL, 0644, 1);

    if (queue_sem != SEM_FAILED)
        cout << "created sem: OK key: " << sem_key  << endl;
    else {
        cerr << "Error creating semaphore!" << endl;
        cerr << strerror(errno) << endl;
    }


}

