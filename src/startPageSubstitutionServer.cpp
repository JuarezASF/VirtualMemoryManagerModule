//
// Created by jasf on 5/23/16.
// ver readme.md para informações

#include <iostream>
#include "PageSubstitutionServer.h"

using namespace std;

int main(int argc, char **argv) {

    string logStr = "[page substitution server]";

    cout << logStr << "start substitution server" << endl;
    PageSubstitutionServer s;
    s.run();

    return 0;

}

