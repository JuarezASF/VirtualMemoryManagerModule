//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H


#include "AbstractProcess.h"

class PageAllocationServer : public AbstractProcess{
protected:

    PageFrameTable *table;
    jLock *tableLock;

    void markFrameAsOcupied(int frame);


public:

    PageAllocationServer(int pidx, bool zeroOutTable);

    void run();


    int getFrameForPage(int page);

    int emptyOldestPage();

    void printTable();






};


#endif //VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
