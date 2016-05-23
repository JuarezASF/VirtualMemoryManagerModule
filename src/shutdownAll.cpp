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

    cout << logStr << "destroying resources" << endl;

    rm->destroyAllResources();

    return 0;

}

