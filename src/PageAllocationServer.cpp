//
// Created by jasf on 5/15/16.
//

#include "PageAllocationServer.h"

PageAllocationServer::PageAllocationServer(int pidx) : AbstractProcess(pidx) {
    logStr = "[page alloc server]";

    cout << logStr << "started" << endl;

}

void PageAllocationServer::run() {

    int i = 0;

    while (!AbstractProcess::quitRequested) {
        jMessageQueue::RequestMessage r = serverRequestQueue->getRequest();

        jMessageQueue::AnswerMsg a;

        cout << logStr << "sending frame " << i++ << " to pid " << r.pid << endl;
        flush(cout);

        jMessageQueue::sendAnswer(i++, serverAnserQueue->msgqid, r.pid);


    }


}

