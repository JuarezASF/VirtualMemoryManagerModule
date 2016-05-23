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

void foo(int d){}

int main(int argc, char **argv) {

    string logStr = "[page allocation server]";

    // create all resources from scratch
    //  this is done on another process for compatilibity with the first verion of this code
    // when everything was done in one huge main
    if (fork() == 0){
        ResourceManager *rm = ResourceManager::getInstance("config.txt");
        rm->createAllResources();

        PIDTable *p = rm->getPIDTable();

        rm->getPIDTableLock()->acquire();
        p->qtdUsedEntries = 0;
        rm->getPIDTableLock()->release();

        rm->printPIDTable();

        exit(0);
    }else{
        int s;
        wait(&s);
    }


    cout << logStr << "start paging server" << endl;
    PageAllocationServer s(10000, true);
    s.run();

    return 0;

}

