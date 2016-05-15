//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H


#include "AbstractProcess.h"

class PageAllocationServer : public AbstractProcess{

public:

    PageAllocationServer(int pidx);

    void run();


};


#endif //VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
