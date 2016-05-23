//
// Created by jasf on 5/15/16.
//

#include "PageAllocationServer.h"

PageAllocationServer::PageAllocationServer(int pidx, bool zeroOutTable) : AbstractProcess(pidx) {
    logStr = "[page alloc server]";


    table = rm->getTable();
    tableLock = rm->getTableLock();

    //zero out table
    if (zeroOutTable) {
        table->qtdFree = NUMERO_FRAMES;
        table->pageFaultCount = 0;
        for (int k = 0; k < NUMERO_FRAMES; k++) {
            table->table[k].page = 0;
            table->table[k].occupied = false;
            table->table[k].timestamp = 0X7FFFFFFFFFFFFFFFL;
        }

    }

    cout << logStr << "started" << endl;

}

void PageAllocationServer::run() {

    while (!AbstractProcess::quitRequested) {
        jMessageQueue::RequestMessage r = serverRequestQueue->getRequest();

        bool pageFault = false;
        int page = r.page;
        flush(cout);

        tableLock->acquire();
        int frame = getFrameForPage(page);
        tableLock->release();
        if (frame < 0) {
            pageFault = true;
            table->pageFaultCount = table->pageFaultCount + 1;
            cout << logStr << "PAGE FAULT on request for page " << page << endl;
            frame = emptyOldestPage();

            if (frame < 0) {
                cerr << "TREMENDOUS ERROR! FIX ME" << endl;
                frame = 0;
            }
        }

        markFrameAsOcupied(frame);
        flush(cout);

        jMessageQueue::sendAnswer(frame, serverAnserQueue->msgqid, r.pid, pageFault);


    }


}

int PageAllocationServer::getFrameForPage(int page) {
    if (table->qtdFree <= 0)
        return -1;

    int out = -1;
    for (int k = 0; k < NUMERO_FRAMES; k++) {
        if (table->table[k].page == page && table->table[k].occupied)
            return k;
        if (!table->table[k].occupied)
            out = k;
    }

    return out;
}

int PageAllocationServer::emptyOldestPage() {
    long oldest = 0x7FFFFFFFFFFFFFFFL;
    int oldestIdx = -1;

    tableLock->acquire();

    for (int k = 0; k < NUMERO_FRAMES; k++) {
        if (table->table[k].occupied && (table->table[k].timestamp <= oldest)) {
            oldestIdx = k;
            oldest = table->table[k].timestamp;
        }
    }

    if (oldestIdx >= 0) {
        table->table[oldestIdx].occupied = false;
        table->qtdFree++;
    }
    else {

        cerr << logStr << "CANNOT FIND OLDEST ENTRY TO DELETE. ERROR! FIX ME USELESS PROGRAMMER" << endl;
    }
    tableLock->release();
    return oldestIdx;
}

void PageAllocationServer::markFrameAsOcupied(int frame) {
    if (frame < 0) {
        cerr << "TREMENDOUS ERROR! FIX ME" << endl;
        frame = 0;
    }
    table->table[frame].occupied = true;
    table->table[frame].timestamp = (long) time(NULL);
    table->qtdFree--;

}

void PageAllocationServer::printTable() {
    cout << "page fault count:" << table->pageFaultCount << endl;
    cout << "qtd frame free:" << table->qtdFree << endl;

    cout << "table" << endl;
    cout << "\tframe\tpage\ttimestamp\toccupied" << endl;
    for (uint k = 0; k < NUMERO_FRAMES; k++) {
        cout << "\t" << k << "\t" << table->table[k].page << "\t" << table->table[k].timestamp << "\t" <<
                ((table->table[k].occupied) ? "true" : "false") << endl;
    }


}
