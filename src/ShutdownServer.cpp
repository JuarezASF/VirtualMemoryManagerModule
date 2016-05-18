//
// Created by jasf on 5/16/16.
//

#include <unistd.h>
#include <string>
#include <iostream>

#include "ShutdownServer.h"
#include "UserProcess.h"

using namespace std;

ShutdownServer::ShutdownServer(int pidx) : AbstractProcess(pidx) {
    logStr = "[shutdown server]";

    qtdPageFaults = 0;

    cout << logStr << "started" << endl;
}

void ShutdownServer::run() {

    quitRequested = 1;
    cout << logStr << "Numero de page faults no processo " << to_string(pidx) <<":" <<endl;
    cout << logStr << "Numero de page faults total: " <<endl;
    cout << logStr << "Numero de execucoes do processo de substituicao: "   <<endl;


}

