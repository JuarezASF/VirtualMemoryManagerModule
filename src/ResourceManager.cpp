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

    pageRequestServerQueu->create(serverQueueKey);
    serverAnswerQueue->create(serverAnswerKey);
    serverQueueLock->create(serverQueueLockKey);

}

void ResourceManager::startAllResources() {
    int serverQueueKey = ConfigParser::getInt("serverQueueKey");
    int serverQueueLockKey = ConfigParser::getInt("serverQueueKey");
    int serverAnswerKey = ConfigParser::getInt("serverAnswerKey");

    pageRequestServerQueu->start(serverQueueKey);
    serverAnswerQueue->start(serverAnswerKey);
    serverQueueLock->start(serverQueueLockKey);

}

void ResourceManager::destroyAllResources() {
    pageRequestServerQueu->destroy();
    serverAnswerQueue->destroy();
    serverQueueLock->destroy();

}


ResourceManager::ResourceManager(string configFilename) {
    ConfigParser::loadConfig(configFilename);
    pageRequestServerQueu = new jMessageQueue();
    serverAnswerQueue = new jMessageQueue();
    serverQueueLock = new jLock();

}

jMessageQueue *ResourceManager::getServerRequestQueue() {
    return pageRequestServerQueu;
}

jLock *ResourceManager::getServerRequestLock() {
    return serverQueueLock;
}

jMessageQueue *ResourceManager::getServerAnswertQueue() {
    return serverAnswerQueue;
}
