//
// Created by jasf on 5/14/16.
//


#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#include <errno.h>
# include <unistd.h>
#include <fcntl.h>           /* For O_* constants */


#include <cstring>
#include <iostream>
#include "ConfigParser.h"
#include "ResourceManager.h"


int semid;

int main(int argc, char **argv) {

    ResourceManager* rm = ResourceManager::getInstance();
    rm->loadConfig("config.txt");

}

