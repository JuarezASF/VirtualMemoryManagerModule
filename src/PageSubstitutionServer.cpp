//
// Created by jasf on 5/16/16.
//

#include <unistd.h>
#include <string>
#include <iostream>

#include "PageSubstitutionServer.h"

using namespace std;

PageSubstitutionServer::PageSubstitutionServer() : PageAllocationServer(false) {
    logStr = "[page subst server]";

    cout << logStr << "started" << endl;
}

void PageSubstitutionServer::run() {

    while (!quitRequested) {
        if (usleep(PAGE_SUBS_SLEEP_TIME) < 0) {
            cerr << strerror(errno) << endl;
        }

        if ((NUMERO_FRAMES - table->qtdFree) > MAX_OCUPACAO) {
            cout << logStr << "Need to free some space! Current ocupation is:" << NUMERO_FRAMES - table->qtdFree <<
            endl;

            //keep count of time the substitution took place inside the shared table
            pidTableLock->acquire();
            pidTable->pageFaultCount[idxOnPIDTable] += 1;
            pidTableLock->release();

            while ((NUMERO_FRAMES - table->qtdFree) > OCUPACAO_OK) {
                this->emptyOldestPage();
            }
            cout << logStr << "Done! Current ocupation is:" << NUMERO_FRAMES - table->qtdFree << endl;
        }

    }


}
