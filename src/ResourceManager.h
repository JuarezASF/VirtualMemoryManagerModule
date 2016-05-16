//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H
#define VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H

#include <string>
#include <iostream>
#include "ConfigParser.h"
#include "jLock.h"
#include "jMessageQueue.h"
#include "structs.h"
#include "jSharesMemory.h"

typedef struct PageFrameTable PageFrameTable;

class ResourceManager {

private:

    int tableSemId;

    ResourceManager(string configFilename);

    jMessageQueue *serverRequestQueue, *serverAnswerQueue;
    jLock *serverQueueLock;
    jSharesMemory *tableSharedMemory;
    PageFrameTable *table;
    jLock *tableLock;

public:

    static ResourceManager *getInstance(string configFilename);
    virtual ~ResourceManager();

    /**
     * Call when you are creating the resources for the very first time
     */
    void createAllResources();

    /**
     * Call when you assume someone else already created the resource
     */
    void startAllResources();


    void destroyAllResources();

    jMessageQueue *getServerRequestQueue();

    jLock *getServerRequestLock();

    jMessageQueue *getServerAnswertQueue();

    PageFrameTable *getTable();

    jLock * getTableLock();


};


#endif //VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H
