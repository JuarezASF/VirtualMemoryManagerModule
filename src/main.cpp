//
// Created by jasf on 5/15/16.
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
#include "ShutdownServer.h"


using namespace std;

void execute_child(int idx);

void foo(int d){}

int main(int argc, char **argv) {
    int qtdProcess = QUANTIDADE_USR_PROCESSES;
    int child_id;
    string logStr = "[main]";

    //initiate resources

    cout << logStr << "Creating resources..." << endl;
    if (fork() == 0){
        ResourceManager *rm = ResourceManager::getInstance("config.txt");
        rm->createAllResources();
        exit(0);
    }else{
        int s;
        wait(&s);
    }

    //start page server
    cout << logStr << "start paging server" << endl;
    int pageServerPID;
    if ((pageServerPID = fork()) == 0){
        PageAllocationServer s(10000, true);
        s.run();
        exit(0);
    }

    //start page substitution server
    cout << logStr << "start substitution server" << endl;
    int substitutionServerPid;
    if ((substitutionServerPid = fork()) == 0){
        PageSubstitutionServer s(10001);
        s.run();
        exit(0);
    }

     cout << logStr << "shutdown process" << endl;
    int ShutdownPid;
    if ((ShutdownPid = fork()) == 0){
        ShutdownServer s(0);
        s.run();
        exit(0);
    }




    unordered_map<int, int> pidIdxMap;
    set<int> childIdxDone;

    for (int i = 0; i < qtdProcess; i++) {
        if ((child_id = fork()) == 0) {
            ChildProcess p(i);
            p.run();
            exit(0);
        } else {
            pidIdxMap[child_id] = i;
            cout << logStr << "started process:#" << i << " pid:" << child_id << endl;
        }
    }

    cout << logStr << "waiting for childs" << endl;
    for (int i = 0; i < qtdProcess; i++) {
        int status;
        int cpid = wait(&status);
        int cidx = pidIdxMap[cpid];

        cout << logStr << "child pid:" << cpid << " idx:" << cidx << " OK" << endl;
    }

    //kill page server
    kill(pageServerPID, SIGUSR2);
    //wait paging server to finish
    int status;
    int cpid = wait(&status);

    //kill substitution server
    kill(substitutionServerPid, SIGUSR2);
    //wait substitution server to finish
    cpid = wait(&status);

    cout << logStr << "DONE" << endl;

    cout << logStr << "Destroying resources ..." << endl;
    if (fork() == 0){
        ResourceManager *rm = ResourceManager::getInstance("config.txt");
        rm->startAllResources();
        rm->destroyAllResources();
        exit(0);
    }else{
        int s;
        wait(&s);
    }




}



