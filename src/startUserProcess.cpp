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
int main(int argc, char **argv){
    string logStr = "[page substitution server]";

    cout << logStr << "start substitution server" << endl;
    if (argc != 2){
        cerr << "usage:" << "./startUsreProcess pag_processo_N.txt" << endl;
        exit(0);
    }
    string filename(argv[1]);
    UserProcess s(filename);
    s.run();

    return 0;

}

