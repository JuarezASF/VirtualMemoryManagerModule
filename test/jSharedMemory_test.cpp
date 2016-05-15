//
// Created by jasf on 5/15/16.
//

#include "../src/jSharesMemory.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

void foo(int d){

}

int main() {

    jSharesMemory m;

    m.create(0x1234, 4);

    int *a = (int *)m.ptr;
    *a = 10;
    int ppid = getpid();
    int cpid;

    if ((cpid = fork()) == 0){

        jSharesMemory m2;

        m2.start(0x1234, 4);
        int *b = (int *)m2.ptr;
        *b = 1993;
        kill(ppid, SIGUSR1);

        signal(SIGUSR2, foo);
        pause();

        *b = 2016;
        kill(ppid, SIGUSR1);

        exit(0);

    }else{
        signal(SIGUSR1, foo);
        pause();

        cout << *a << endl;

        kill(cpid, SIGUSR2);

        int s;
        wait(&s);

        cout << *a << endl;


    }

    m.destroy();


}
