//
// Created by jasf on 5/15/16.
//

#include <unistd.h>
#include "AbstractProcess.h"

bool AbstractProcess::quitRequested = false;
string AbstractProcess::logStr = "[NOT A NAME]";

AbstractProcess::AbstractProcess(int pidx) {

    signal(SIGUSR2, AbstractProcess::onSIGUSR2);

    logStr = "[pidx:" + to_string(pidx) + "]";
    rm = ResourceManager::getInstance("config.txt");
    rm->startAllResources();

    serverRequestQueue = rm->getServerRequestQueue();
    serverAnserQueue = rm->getServerAnswertQueue();
    serverQueueLock = rm->getServerRequestLock();

    pid = getpid();
    ppid = getppid();
    this->pidx = pidx;

    PIDTable * pidTable = rm->getPIDTable();
    rm->getPIDTableLock()->acquire();
    pidTable->pids[pidTable->qtdUsedEntries] = getpid();
    pidTable->qtdUsedEntries++;
    rm->getPIDTableLock()->release();



}

void AbstractProcess::onSIGUSR2(int d) {
    AbstractProcess::quitRequested = true;
    cout << logStr <<  "quit requested" << endl;
    exit(0);
}
