//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_JSHARESMEMORY_H
#define VIRTUALMEMORYMANAGER_JSHARESMEMORY_H


class jSharesMemory {

public:

    int id;
    void *ptr;

    jSharesMemory();

    void *create(int key, int size);

    void *start(int key, int size);

    void destroy();

    void deatach();

};


#endif //VIRTUALMEMORYMANAGER_JSHARESMEMORY_H
