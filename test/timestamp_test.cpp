//
// Created by jasf on 5/14/16.
//

#include <ctime>
#include <iostream>
#include <unistd.h>

using namespace std;

int main(){

    for(int i = 0 ; i < 10; i++){
        long t = time(NULL);
        sleep(1);
        cout << t << endl;
    }
}

