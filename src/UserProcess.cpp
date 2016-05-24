//
// Created by jasf on 5/15/16.
//

#include "UserProcess.h"

using namespace std;

UserProcess::UserProcess(string filename) : AbstractProcess() {
    is.open(filename.c_str());
    if (!is.is_open()) {
        cerr << logStr << "could not open file " << filename << endl;
    }


}

void UserProcess::run() {
    int pag;
    while (is.is_open()) {
        usleep(USER_USLEEP_TIPE);
        if (is.eof())
            break;
        is >> pag;
        //ignore comma
        is.ignore();

        referencia_pagina(pag);
    }

    cout << logStr << "done" << endl;
    exit(0);


}

void UserProcess::referencia_pagina(int page) {
    serverQueueLock->acquire();
    serverRequestQueue->sendRequest(page, serverAnserQueue->msgqid);

    jMessageQueue::AnswerMsg a = serverAnserQueue->getAnswer(this->pid);

    cout << logStr << "page " << page << " mapped to frame " << a.frame;
    if (a.pagefault)
        cout << " with pagefault";
    cout << endl;

    if (a.pagefault) {
        // increase pagefault count on this user process
        rm->getPIDTableLock()->acquire();
        pidTable->pageFaultCount[idxOnPIDTable] += 1;
        rm->getPIDTableLock()->release();

    }
    serverQueueLock->release();

}
