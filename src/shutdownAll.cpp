//
// Created by jasf on 5/23/16.
//
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <unordered_map>
#include <set>
#include "defines.h"
#include "UserProcess.h"
#include "PageAllocationServer.h"
#include "PageSubstitutionServer.h"

using namespace std;

void execute_child(int idx);

void foo(int d) { }

int main(int argc, char **argv) {

    string logStr = "[shutdown]";

    ResourceManager *rm = ResourceManager::getInstance("config.txt");
    rm->startAllResources();

    PIDTable *pidTable = rm->getPIDTable();

    rm->printPIDTable();

    for (uint k = 0; k <  pidTable->qtdUsedEntries; k++ ){
        long toKillPid = pidTable->pids[k];

        kill(toKillPid, SIGUSR2);
    }

    cout << "[Required Execution Log]start" << endl;

    for (uint k = 2; k < pidTable->qtdUsedEntries; k++){
        cout << "Numero de page faults do processo " << k-2 << ":" << pidTable->pageFaultCount[k] << endl;

    }

    cout << "Numero de page faults total:" << pidTable->pageFaultCount[0] << endl;

    cout << "Numero de execucoes do processo de substituicao:" << pidTable->pageFaultCount[1] << endl;

    PageFrameTable *t = rm->getTable();
    cout << "Configuracao final da memoria" << endl;
    cout << "Pagina\tTempo(relativo) de referencia" << endl;
    for (uint k = 0; k < NUMERO_FRAMES; k++){
        cout << t->table[k].page << "\t" << t->table[k].timestamp - pidTable->initialTimestamp << endl;

    }

    cout << "[Required Execution Log]end" << endl;

    cout << logStr << "destroying resources" << endl;

    rm->destroyAllResources();

    return 0;

}

