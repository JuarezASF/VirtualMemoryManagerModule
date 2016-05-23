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
typedef struct PIDTable PIDTable;

class ResourceManager {

private:

    int tableSemId;

    ResourceManager(string configFilename);

    jMessageQueue *serverRequestQueue, *serverAnswerQueue;
    jLock *serverQueueLock;
    jSharesMemory *tableSharedMemory, *pidSharedMemory;
    PageFrameTable *table;
    PIDTable *pidTable;
    jLock *tableLock, *pidSharedMemoryLock;

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

    PIDTable *getPIDTable();
    jLock *getPIDTableLock();

    void printPIDTable();


};


#endif //VIRTUALMEMORYMANAGER_RESOURCEMANAGER_H
