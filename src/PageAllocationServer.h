//
// Created by jasf on 5/15/16.
//

#ifndef VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
#define VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H


#include "AbstractProcess.h"

class PageAllocationServer : public AbstractProcess {
protected:

    typedef struct frameInfo {
        int frame;
        bool pagefault;

        frameInfo(int d, bool b) : frame(d), pagefault(b) { }
    } frameInfo;

    PageFrameTable *table;
    jLock *tableLock;
    long timeCounter;


    /***
     * set frame to page, marks it as occupied and the last accessed time to the current timestamp
     *
     * LOCKS THE TABLE WHILE MODIFICATIONS ARE BEING DONE
     */
    void markFrameAsOcupied(int frame, int page);


public:

    PageAllocationServer(bool zeroOutTable);

    /**
     * Process main loop:
     *     fetches msg from the input queue, find a page of it and send the answert to the output queue.
     *
     *     In case of no free frame, remove the oldest entry and allocate to the page being requested.
     */
    void run();


    /**
     * Returns frame where the page can be allocated in struct of type (int frame, bool pagefaults)
     * This does not change the table!
     * If the page is already in the table, returns the frame and pagefault false
     * If the page in not on the table but there is a free frame, returns the available frame and pagefault true
     * it the page is not on the table and there is space, returns -1 and pagefault true
     *
     * DO NOT LOCK THE TABLE! REMIND TO LOCK IT YOURSERL IF NECESSARY!
     * This is done because this method only reads the table and you may not want to synchronize reads
     */
    frameInfo getFrameForPage(int page);

    /**
     * Find oldest occupied entry and set it to not occupied. Returns the position that
     * was free. Note we necessarily look for an occupied position.
     *
     * THIS METHODS LOCKS THE TABLE
     */
    int emptyOldestPage();

    void printTable();


};


#endif //VIRTUALMEMORYMANAGER_PAGEALLOCATIONSERVER_H
