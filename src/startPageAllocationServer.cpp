//
// Created by jasf on 5/23/16.
// ver readme.md para informações


#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include "UserProcess.h"
#include "PageAllocationServer.h"

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
        p->initialTimestamp = time(NULL);
        rm->getPIDTableLock()->release();

        exit(0);
    }else{
        int s;
        wait(&s);
    }


    cout << logStr << "start paging server" << endl;
    PageAllocationServer s(true);
    s.run();

    return 0;

}

