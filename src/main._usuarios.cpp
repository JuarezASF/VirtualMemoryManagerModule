//
// Modified by gfbm on 5/20/16.
//

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>
#include <unordered_map>
#include <set>
#include "defines.h"
#include "UserProcess.h"
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
   
    int status;
    int cpid = wait(&status);

    
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



