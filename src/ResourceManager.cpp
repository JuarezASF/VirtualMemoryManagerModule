//
// Created by jasf on 5/14/16.
//

#include "ResourceManager.h"


ResourceManager *ResourceManager::getInstance(string configFilename) {
    static ResourceManager *instance = new ResourceManager(configFilename);
    return instance;
}


void ResourceManager::createAllResources() {
    int serverQueueKey = ConfigParser::getInt("serverQueueKey");
    int serverQueueLockKey = ConfigParser::getInt("serverQueueKey");
    int serverAnswerKey = ConfigParser::getInt("serverAnswerKey");

    serverRequestQueue->create(serverQueueKey);
    serverAnswerQueue->create(serverAnswerKey);
    serverQueueLock->create(serverQueueLockKey);

    int tableSharedMemKey = ConfigParser::getInt("tableSharedMemoryKey");
    int tableLockKey = ConfigParser::getInt("tableLockKey");
    tableSharedMemory->create(tableSharedMemKey, sizeof(PageFrameTable));
    table = (PageFrameTable *) tableSharedMemory->ptr;
    tableLock->create(tableLockKey);

    int pidTableKey = ConfigParser::getInt("sharedPIDTableKey");
    pidSharedMemory->create(pidTableKey, sizeof(PIDTable));
    pidTable = (PIDTable *) pidSharedMemory->ptr;

    int pidTableLockKey = ConfigParser::getInt("sharedPIDTableLockKey");
    pidSharedMemoryLock->create(pidTableLockKey);


}

void ResourceManager::startAllResources() {
    int serverQueueKey = ConfigParser::getInt("serverQueueKey");
    int serverQueueLockKey = ConfigParser::getInt("serverQueueKey");
    int serverAnswerKey = ConfigParser::getInt("serverAnswerKey");

    serverRequestQueue->start(serverQueueKey);
    serverAnswerQueue->start(serverAnswerKey);
    serverQueueLock->start(serverQueueLockKey);

    int tableSharedMemKey = ConfigParser::getInt("tableSharedMemoryKey");
    int tableLockKey = ConfigParser::getInt("tableLockKey");
    tableSharedMemory->start(tableSharedMemKey, sizeof(PageFrameTable));
    table = (PageFrameTable *) tableSharedMemory->ptr;
    tableLock->start(tableLockKey);

    int pidTableKey = ConfigParser::getInt("sharedPIDTableKey");
    pidSharedMemory->start(pidTableKey, sizeof(PIDTable));
    pidTable = (PIDTable *) pidSharedMemory->ptr;

    int pidTableLockKey = ConfigParser::getInt("sharedPIDTableLockKey");
    pidSharedMemoryLock->start(pidTableLockKey);

}

void ResourceManager::destroyAllResources() {
    serverRequestQueue->destroy();
    serverAnswerQueue->destroy();
    serverQueueLock->destroy();

    tableSharedMemory->deatach();
    tableSharedMemory->destroy();

    tableLock->destroy();

    table = nullptr;

    pidSharedMemory->destroy();
    pidSharedMemoryLock->destroy();

    pidTable = nullptr;

}


ResourceManager::ResourceManager(string configFilename) {
    ConfigParser::loadConfig(configFilename);
    serverRequestQueue = new jMessageQueue();
    serverAnswerQueue = new jMessageQueue();
    serverQueueLock = new jLock();
    tableLock = new jLock();
    tableSharedMemory = new jSharesMemory();

    table = nullptr;

    pidSharedMemory = new jSharesMemory();
    pidSharedMemoryLock = new jLock();

}

ResourceManager::~ResourceManager() {
    delete serverRequestQueue;
    delete serverAnswerQueue;
    delete serverQueueLock;
    delete tableLock;
    delete tableSharedMemory;
    delete pidSharedMemory;
    delete pidSharedMemoryLock;

}

jMessageQueue *ResourceManager::getServerRequestQueue() {
    return serverRequestQueue;
}

jLock *ResourceManager::getServerRequestLock() {
    return serverQueueLock;
}

jMessageQueue *ResourceManager::getServerAnswertQueue() {
    return serverAnswerQueue;
}

PageFrameTable *ResourceManager::getTable() {
    return table;
}

jLock *ResourceManager::getTableLock() {
    return tableLock;
}

PIDTable *ResourceManager::getPIDTable() {
    return pidTable;
}

jLock *ResourceManager::getPIDTableLock() {
    return pidSharedMemoryLock;
}

void ResourceManager::printPIDTable() {

    cout << "PID TABLE" << endl;

    int qtd = pidTable->qtdUsedEntries;
    cout << "qtd used entries: " << qtd << endl;

    cout << "used pids" << endl;
    for (uint k = 0; k < qtd; k++)
        cout << pidTable->pids[k] << ",";
    cout << endl;

}
