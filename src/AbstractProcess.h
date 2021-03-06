//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_ABSTRACTPROCESS_H
#define VIRTUALMEMORYMANAGER_ABSTRACTPROCESS_H

#include <fstream>
#include <string>
#include "ResourceManager.h"
#include <sys/signal.h>

using namespace std;

class AbstractProcess {
protected:

    int pid, ppid;
    static string logStr;
    ResourceManager *rm;
    jMessageQueue *serverRequestQueue, *serverAnserQueue;
    jLock *serverQueueLock, *pidTableLock;
    PIDTable *pidTable;


    int idxOnPIDTable;


public:
    AbstractProcess();

    virtual void run() = 0;

    static void onSIGUSR2(int d);

    static bool quitRequested;
};


#endif //VIRTUALMEMORYMANAGER_ABSTRACTPROCESS_H
