//
// Created by jasf on 5/14/16.
//

#include "ResourceManager.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#include <errno.h>
# include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include "structs.h"
#include "jLock.h"


#include <cstring>

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
