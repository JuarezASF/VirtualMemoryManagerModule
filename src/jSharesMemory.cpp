//
// Created by jasf on 5/15/16.
//

#include "jSharesMemory.h"
#include <sys/shm.h>
#include <iostream>
#include <string.h>

using namespace std;

jSharesMemory::jSharesMemory() {
    id = -1;
    ptr = nullptr;

}

void *jSharesMemory::create(int key, int size) {
    if ((this->id = shmget(key, size, IPC_EXCL | IPC_CREAT | 0664)) < 0) {
        cerr << strerror(errno) << endl;
        cerr << "Trying to get previously created shared memory!" << endl;

        if ((this->id = shmget(key, size, IPC_CREAT | 0664)) < 0) {
            cerr << strerror(errno) << endl;
        }
    }
    if (id > 0) {
        ptr = (void *) shmat(id, 0, 0);
        if (ptr < 0) {
            cerr << strerror(errno) << endl;
        }

    }
}

void *jSharesMemory::start(int key, int size) {

    if ((this->id = shmget(key, size, IPC_CREAT | 0664)) < 0) {
        cerr << strerror(errno) << endl;
    }
    if (id > 0) {
        ptr = (void *) shmat(id, 0, 0);
        if (ptr < 0) {
            cerr << strerror(errno) << endl;
        }

    }
}

void jSharesMemory::destroy() {

    if (shmctl(id, IPC_RMID, 0) < 0)
        cerr << strerror(errno) << endl;

}

void jSharesMemory::deatach() {

    if (shmdt(ptr) < 0)
        cerr << strerror(errno) << endl;

}
