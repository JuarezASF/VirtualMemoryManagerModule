//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H


#include "AbstractProcess.h"

class PageAllocationServer : public AbstractProcess{
protected:

    typedef struct frameInfo{
        int frame;
        bool pagefault;

        frameInfo(int d, bool b) : frame(d), pagefault(b){}
    }frameInfo;

    PageFrameTable *table;
    jLock *tableLock;

    void markFrameAsOcupied(int frame, int page);


public:

    PageAllocationServer(bool zeroOutTable);

    void run();


    frameInfo getFrameForPage(int page);

    int emptyOldestPage();

    void printTable();






};


#endif //VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
