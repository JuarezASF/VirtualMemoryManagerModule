//
// Created by jasf on 5/16/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H

#include "PageAllocationServer.h"

class PageSubstitutionServer : public PageAllocationServer {
public:
    PageSubstitutionServer();

    /**
     * Page substitution main loop:
     *
     * every X microseconds, check the number of occupied frames. If it is greater o equal to
     * MAX_OCUPACAO starts to remove the oldest entries one at a time untill ocuppation reaches
     * OCUPACAO_OK
     */
    void run();


};


#endif //VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H
