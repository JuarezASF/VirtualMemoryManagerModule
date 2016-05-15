//
// Created by jasf on 5/14/16.
//

#include "ResourceManager.h"

#include "jLock.h"


ResourceManager *ResourceManager::getInstance() {
    static ResourceManager *instance = new ResourceManager();
    return instance;
}

void ResourceManager::loadConfig(string filename) {
    ConfigParser::loadConfig(filename);

}

ResourceManager::ResourceManager() {

}


void ResourceManager::initAllResources() {
    int sem_key = ConfigParser::getInt("semkey");

    jLock tableLock;
    tableLock.create(sem_key);

}

void ResourceManager::clearAllResources() {

}

jLock ResourceManager::getTableLock() {
    int sem_key = ConfigParser::getInt("semkey");

    jLock tableLock;
    tableLock.start(sem_key);
    tableLock;
}
