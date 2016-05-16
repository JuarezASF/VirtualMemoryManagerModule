//
// Created by jasf on 5/16/16.
//

#include <unistd.h>
#include <string>
#include <iostream>

#include "PageSubstitutionServer.h"

using namespace std;

PageSubstitutionServer::PageSubstitutionServer(int pidx) : PageAllocationServer(pidx, false) {
    logStr = "[page subst server]";

    qtdSubstituitonTookPlace = 0;

    cout << logStr << "started" << endl;
}

void PageSubstitutionServer::run() {

    while (!quitRequested) {
        if (sleep(PAGE_SUBS_SLEEP_TIME) < 0) {
            cerr << strerror(errno) << endl;
        }

        if (table->qtdFree >= MAX_OCUPACAO) {
            cout << logStr << "starting to free some space" << endl;
            qtdSubstituitonTookPlace++;
            while (table->qtdFree >= OCUPACAO_OK)
                this->emptyOldestPage();
        }

    }


}
