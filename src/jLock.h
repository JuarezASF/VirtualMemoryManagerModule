//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_JLOCK_H
#define VIRTUALMEMORYMANAGER_JLOCK_H


using namespace std;

class jLock {
    static struct sembuf semAcquire[1];
    static struct sembuf semRelease;

private:
    int semid;
    int key;

public:

    jLock();
    virtual ~jLock();

    void create(int key);

    void start(int key);

    void destroy();

    void acquire();

    void release();

    int getCurrentVal();


};


#endif //VIRTUALMEMORYMANAGER_JLOCK_H
