//
// Created by jasf on 5/14/16.
//

#ifndef VIRTUALMEMORYMANAGER_STRUCTS_H
#define VIRTUALMEMORYMANAGER_STRUCTS_H

#include "ResourceManager.h"
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>

#include <errno.h>
# include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include "defines.h"


#include <cstring>


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

typedef struct TableEntry {
    int page;
    long timestamp;
    bool occupied;
} TableEntry;

typedef struct PageFrameTable {
    int qtdFree;
    TableEntry table[NUMERO_FRAMES];
} PageFrameTable;

typedef struct PIDTable {
    int qtdUsedEntries;
    long pids[MAX_NUMERO_PROCESSOS];
    long pageFaultCount[MAX_NUMERO_PROCESSOS];
    long initialTimestamp;
} PIDTable;

#endif //VIRTUALMEMORYMANAGER_STRUCTS_H
