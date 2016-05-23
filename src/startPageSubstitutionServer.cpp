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

int main(int argc, char **argv) {

    string logStr = "[page substitution server]";

    cout << logStr << "start substitution server" << endl;
    PageSubstitutionServer s;
    s.run();

    return 0;

}

