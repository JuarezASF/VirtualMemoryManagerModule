//
// Created by jasf on 5/14/16.
//
#include <ios>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "ConfigParser.h"

#include "ConfigParser.h"

int main (int argc, char ** argv){

   ConfigParser::loadConfig("config.txt");

   int sem_key = ConfigParser::getInt( "PAPQueueSemaphorKey");
   //check
   int PAPQueueSemaphoreID = semget(sem_key, 0, 0xFFF);

   if (PAPQueueSemaphoreID >= 0){
      cerr << "Destroying semaphore on id " << sem_key << endl;
      semctl(PAPQueueSemaphoreID, 0, IPC_RMID);
   }
   else{
      cerr << "Semaphore was not created! This is probably a mistake!" << endl;
   }

}