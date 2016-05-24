//
// Created by jasf on 5/15/16.
//

#include "PageAllocationServer.h"

PageAllocationServer::PageAllocationServer(bool zeroOutTable) : AbstractProcess() {
    logStr = "[page alloc server]";


    table = rm->getTable();
    tableLock = rm->getTableLock();
    timeCounter = 0;

    //zero out table
    if (zeroOutTable) {
        table->qtdFree = NUMERO_FRAMES;
        for (int k = 0; k < NUMERO_FRAMES; k++) {
            table->table[k].page = 0;
            table->table[k].occupied = false;
            table->table[k].timestamp = 0;
        }

    }

    //it does not make sense to keep page count for allocation and substitution server
    PIDTable *pidTable = rm->getPIDTable();
    pidTable->pageFaultCount[idxOnPIDTable] = 0;

}

void PageAllocationServer::run() {

    while (!AbstractProcess::quitRequested) {
        jMessageQueue::RequestMessage r = serverRequestQueue->getRequest();

        bool pageFault = false;
        int allocatedFrame;
        int page = r.page;

        //table read operations are synchronized: we don't want the substitution server writing while we're reading
        tableLock->acquire();
        frameInfo info = getFrameForPage(page);
        tableLock->release();

        pageFault = info.pagefault;
        allocatedFrame = info.frame;
        if (info.pagefault) {
            cout << logStr << "PAGE FAULT on request for page " << page << endl;

            //increase count on total pagefault count at the PIDTable
            pidTableLock->acquire();
            pidTable->pageFaultCount[idxOnPIDTable] += 1;
            pidTableLock->release();

            //means there is a page frame and we don't know where to place the new entry
            if (info.frame < 0) {
                allocatedFrame = emptyOldestPage();
            }
            if (allocatedFrame < 0) {
                // should never happens(if we caught all bugs)
                cerr << "TREMENDOUS ERROR! FIX ME" << endl;
                allocatedFrame = 0;
            }

            cout << logStr << "PAGE FAULT resolved by allocating frame " << allocatedFrame << " for page" << page <<
            endl;

        }

        markFrameAsOcupied(allocatedFrame, page);

        jMessageQueue::sendAnswer(allocatedFrame, serverAnserQueue->msgqid, r.pid, pageFault);

    }


}

PageAllocationServer::frameInfo PageAllocationServer::getFrameForPage(int page) {
    int out = -1;

    for (int k = NUMERO_FRAMES - 1; k >= 0; k--) {
        if (table->table[k].page == page && table->table[k].occupied)
            return PageAllocationServer::frameInfo(k, false);
        if (!table->table[k].occupied)
            out = k;
    }

    return PageAllocationServer::frameInfo(out, true);
}

int PageAllocationServer::emptyOldestPage() {
    long oldest = 0x7FFFFFFFFFFFFFFFL;
    int oldestIdx = -1;

    //since we're to change the table, we synchronize the access
    tableLock->acquire();

    for (int k = 0; k < NUMERO_FRAMES; k++) {
        if (table->table[k].occupied && (table->table[k].timestamp <= oldest)) {
            oldestIdx = k;
            oldest = table->table[k].timestamp;
        }
    }

    if (oldestIdx >= 0) {
        if (table->table[oldestIdx].occupied) {
            table->table[oldestIdx].occupied = false;
            table->qtdFree += 1;
        }
        else {
            cerr << logStr << "ERROR! TRYING TO FREE A NON OCCUPIED FRAME! THIS IS PROBABLY AN ERROR!" << endl;
        }
    }
    else {

        cerr << logStr << "CANNOT FIND OLDEST ENTRY TO DELETE. ERROR! FIX ME USELESS PROGRAMMER" << endl;
    }
    tableLock->release();
    return oldestIdx;
}

void PageAllocationServer::markFrameAsOcupied(int frame, int page) {
    if (frame < 0) {
        cerr << "TREMENDOUS ERROR! FIX ME" << endl;
        frame = 0;
    }
    tableLock->acquire();
    table->table[frame].page = page;
    table->table[frame].timestamp = timeCounter++;
    if (!table->table[frame].occupied) {
        table->qtdFree -= 1;
    }
    table->table[frame].occupied = true;
    tableLock->release();

}

void PageAllocationServer::printTable() {
    cout << "qtd frame free:" << table->qtdFree << endl;

    cout << "table" << endl;
    cout << "\tframe\tpage\ttimestamp\toccupied" << endl;
    for (uint k = 0; k < NUMERO_FRAMES; k++) {
        cout << "\t" << k << "\t" << table->table[k].page << "\t" << table->table[k].timestamp << "\t" <<
        ((table->table[k].occupied) ? "true" : "false") << endl;
    }


}
