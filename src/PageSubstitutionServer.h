//
// Created by jasf on 5/16/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H

#include "PageAllocationServer.h"

class PageSubstitutionServer : public PageAllocationServer{
public:
    PageSubstitutionServer(int pidx);
    int qtdSubstituitonTookPlace;

    void run();


};


#endif //VIRTUALMEMORYMANAGER_PAGESUBSTITUTIONSERVER_H
