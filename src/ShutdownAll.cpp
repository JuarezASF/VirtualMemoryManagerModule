//
// Created by jasf on 5/14/16.
//
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

#include <ios>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "ConfigParser.h"

int main (int argc, char ** argv){

   ConfigParser::loadConfig("config.txt");

   string sem_key = ConfigParser::getString( "PAPQueueSemaphorKey");
   //check
   sem_t *queue_sem = sem_open(sem_key.c_str(), 0, 0644);

   if (queue_sem != SEM_FAILED){
      cout << "Destroying semaphore on id " << sem_key << endl;
      sem_close(queue_sem);
      sem_unlink(sem_key.c_str());
   }
   else{
      cerr << "Semaphore was not found! This is probably a mistake!" << endl;
   }

}