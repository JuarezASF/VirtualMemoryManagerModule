
//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_SHUTDOWNSERVER_H
#define VIRTUALMEMORYMANAGER_SHUTDOWNSERVER_H


#include "AbstractProcess.h"

class ShutdownServer : public AbstractProcess{
protected:

    PageFrameTable *table;
    jLock *tableLock;
    int qtdPageFaults;



public:

    ShutdownServer(int pidx);

    void run();

};


#endif //VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
