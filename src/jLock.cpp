//
// Created by jasf on 5/14/16.
//

#include <sys/sem.h>
#include <iostream>

#include "jLock.h"
#include "structs.h"

struct sembuf jLock::semAcquire[1] = {{0, -1, 0}};
struct sembuf jLock::semRelease = {0, 1, 0};

jLock::jLock() {

}

void jLock::create(int key) {
    int attempt = 3;
    bool semOk = false;
    if ((semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0644)) < 0) {
        cerr << "Error creating semaphore!" << endl;
        cerr << strerror(errno) << endl;
    } else {
        semOk = true;
    }

    if (!semOk && (semid = semget(key, 1, IPC_CREAT | 0644)) < 0) {
        cerr << "Could not even get old sempahore!" << endl;
        cerr << strerror(errno) << endl;
        exit(0);
    }
    semun setTo1;
    setTo1.val = 1;

    if (semctl(semid, 0, SETVAL, setTo1) < 0) {
        cerr << strerror(errno) << endl;
    }


}

void jLock::start(int key) {
    if ((semid = semget(key, 1, 0644)) < 0) {
        cerr << "Error creating semaphore!" << endl;
        cerr << strerror(errno) << endl;

    }

}

void jLock::destroy() {

    if (semid > 0) {
        if (semctl(semid, 0, IPC_RMID) < 0) {
            cerr << strerror(errno) << endl;
        }

    } else {
        cerr << "semaphore has not yet being initialized!" << endl;
    }

}

void jLock::acquire() {

    flush(cout);

    if (semop(semid, semAcquire, 1) < 0)
        cerr << strerror(errno) << endl;

}

void jLock::release() {
    flush(cout);
    if (semop(semid, &semRelease, 1) < 0)
        cerr << strerror(errno) << endl;

}

jLock::~jLock() {

}

int jLock::getCurrentVal() {
    int val = -1;
    if ((val = semctl(semid, 0, GETVAL)) < 0)
        cerr << strerror(errno) << endl;

    return val;
}
