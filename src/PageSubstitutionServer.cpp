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
        if (usleep(PAGE_SUBS_SLEEP_TIME) < 0) {
            cerr << strerror(errno) << endl;
        }

        if ((NUMERO_FRAMES - table->qtdFree) >= MAX_OCUPACAO) {
            cout << logStr << "starting to free some space" << endl;
            qtdSubstituitonTookPlace++;
            while ((NUMERO_FRAMES - table->qtdFree) >= OCUPACAO_OK){
                printTable();
                this->emptyOldestPage();

            }
        }

    }


}
