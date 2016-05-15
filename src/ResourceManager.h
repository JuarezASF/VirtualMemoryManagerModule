//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H
#define VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H

#include <string>
#include <iostream>
#include "ConfigParser.h"
#include "jLock.h"


class ResourceManager {

private:

    int tableSemId;

    ResourceManager();

public:

    void loadConfig(string filename);
    static ResourceManager* getInstance();

    void initAllResources();

    void clearAllResources();

    /**
     * Assumes another process has already created the lock
     */
    jLock getTableLock();


};


#endif //VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H
