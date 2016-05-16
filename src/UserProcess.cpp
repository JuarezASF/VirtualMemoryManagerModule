//
// Created by jasf on 5/15/16.
//

#include "UserProcess.h"
#include "ConfigParser.h"
#include <iostream>

using namespace std;

ChildProcess::ChildProcess(int pidx) : AbstractProcess(pidx){
    string filename = "pag_processo_" + to_string(pidx) + ".txt";
    is.open(filename.c_str());
    if (!is.is_open()) {
        cerr << logStr << "could not open file " << filename << endl;
    }


}

void ChildProcess::run() {
    int pag;
    while (is.is_open()) {
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

void ChildProcess::referencia_pagina(int page) {
    serverQueueLock->acquire();
    serverRequestQueue->sendRequest(page, serverAnserQueue->msgqid);

    jMessageQueue::AnswerMsg a = serverAnserQueue->getAnswer(this->pid);

    cout << logStr << "page " << page << " mapped to frame " << a.frame << " with pagefault " << ((a.pagefault)? "TRUE" : "FALSE") << endl;
    flush(cout);
    serverQueueLock->release();

}
