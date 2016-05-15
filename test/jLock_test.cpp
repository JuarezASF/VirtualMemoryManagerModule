//
// Created by jasf on 5/14/16.
//

#include "../src/jLock.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {

    int key1 = 0x07;
    int key2 = 0x08;
    jLock l1, l2;

    l1.create(key1);
    l2.create(key2);



    int fd[2];

    pipe(fd);
    char buf[10];

    if (fork() == 0) {
        close(fd[0]);

        for (int i = 0; i < 100; i++) {
            l1.acquire();
            cout << "producing " << i << endl;
            flush(cout);
            ((int *) buf)[0] = i;
            write(fd[1], buf, 4);

        }
        ((int *) buf)[0] = -1;
        write(fd[1], buf, 4);

        exit(0);


    } else {
        close(fd[1]);


        int d;
        do {
            read(fd[0], buf, 4);
            d = ((int *) buf)[0];

            cout << "consuming " << d << endl;
            flush(cout);

            l1.release();
        } while (d >= 0);

        wait(&d);

    }

    l1.destroy();
    l2.destroy();


    cout << "end" << endl;

}