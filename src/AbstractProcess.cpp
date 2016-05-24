//
// Created by jasf on 5/15/16.
//

#include <unistd.h>
#include "AbstractProcess.h"

bool AbstractProcess::quitRequested = false;
string AbstractProcess::logStr = "[NOT A NAME]";

AbstractProcess::AbstractProcess() {

    //register SIGUSR2 to terminate process
    signal(SIGUSR2, AbstractProcess::onSIGUSR2);

    //logStr is used to log
    logStr = "[pid:" + to_string(getpid()) + "]";

    //start resources(assumes someone else created them)
    rm = ResourceManager::getInstance("config.txt");
    rm->startAllResources();

    //initiate fields shared by all process
    serverRequestQueue = rm->getServerRequestQueue();
    serverAnserQueue = rm->getServerAnswertQueue();
    serverQueueLock = rm->getServerRequestLock();
    pidTable = rm->getPIDTable();
    pidTableLock = rm->getTableLock();

    //always good to have this information available
    pid = getpid();
    ppid = getppid();

    //add pid on shared pid table
    //synchronized since we're adding new entries to shared pid table
    pidTableLock->acquire();

    idxOnPIDTable = pidTable->qtdUsedEntries;
    pidTable->pids[idxOnPIDTable] = getpid();
    pidTable->pageFaultCount[idxOnPIDTable] = 0;
    pidTable->qtdUsedEntries++;

    pidTableLock->release();


}

void AbstractProcess::onSIGUSR2(int d) {
    AbstractProcess::quitRequested = true;
    cout << logStr << "quit requested" << endl;
    exit(0);
}
