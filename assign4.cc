
//***************************************************************************
//
//
//  Purpose: The purpose of this program is to solve the Producer-Consumer problem using
//  Pthreads Library. Also to get practice using mutex and semaphores in the Pthreads Library.
//
//***************************************************************************
#include <bits/stdint-intn.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <queue>

//number of threads, ITERATION_NUMBER, and number of cookies we can hold
#define NUM_C_THREADS 4 //number of consumer threads
#define NUM_P_THREADS 4 //number of producer threads
#define ITERATION_NUMBER 5 // how many times will iterate on for loops
#define BUFFER_SIZE 10 //max size of the buffer

//semaphore and mutex init
sem_t Nfull;
sem_t Nempty;
pthread_mutex_t mutex;

//variable to keep track of cookies
int16_t cookieCnt = 0;

/**
 * insert_item(int threadID)
 *  this function adds 1 to the cookie counter to simulate a buffer.
 *
 * @param threadID, the ID of the thread doing the "insertion".
 *
 * @return returns 0 if the cookie counter > 0 & less than 10.
 ********************************************************************************/

int insert_item(int threadID){
 
  pthread_mutex_lock(&mutex);
  cookieCnt++; //increment teh cookie counter
  std::cout << "Producer #" << threadID << " inserted a cookie. Total: " << cookieCnt << std::endl;
  pthread_mutex_unlock(&mutex);
  // exit if cookie coutner is greater than buffer size
  if(cookieCnt > BUFFER_SIZE){
    return -1;
  }
  return 0;
} 

/**
 * removeItem(int threadID){
 * this function removes 1 to the cookie counter to simulate a buffer.
 *
 * @param threadID, the ID of the thread doing the "insertion".
 *
 * @return returns 0 if the cookie counter > 0 & less than 10.
 *
 ********************************************************************************/

int removeItem(int threadID){

  pthread_mutex_lock(&mutex);
  cookieCnt--; //remove 1 from cookie nubmer
  std::cout << "Consumer #" << threadID << " removed a cookie.  total: " << cookieCnt << std::endl;
  pthread_mutex_unlock(&mutex);
 // return if its negative
  if(cookieCnt < 0){
    return -1;
  }
  return 0;
}

/**
 * *producer(void* producerID )
 * this function adds 1 to the counter.
 *
 * @param producerID, the ID of the thread doing the "insertion".
 *
 * @return nothing
 *
 ********************************************************************************/

void *producer(void* producerID ){

  //get the I of producer
  long int threadID = (long int) producerID;

  //iterate 5 times
  for (size_t i = 0; i < ITERATION_NUMBER; i++) {

    sem_wait(&Nempty);
    //check if enything went wrong
    if(insert_item(threadID) == -1) {
      std::cout << "Something went wrong... there is overflow.\n";
      exit(-1);
    }
    sem_post(&Nfull);
    sleep(1); //sleep for 1 sec
  }
  //exit
  pthread_exit(NULL);
}

/**
 * *consumer(void* consumerID )
 *  this function removes 1 to the counter
 *
 * @param consumerID, the ID of the thread doing the "removal".
 *
 * @return nothing
 *
 ********************************************************************************/
void *consumer(void* consumerID ){

  //get the ID
  long int threadID = (long int) consumerID;

  //do thread work based on ITERATION_NUMBER
  for (size_t i = 0; i < ITERATION_NUMBER; i++) {
    sem_wait(&Nfull);
    // do work and check if anything went wrong.
    if(removeItem(threadID) == -1){

      std::cout << "Something went wrong... there is underflow.\n";
      exit(-1);
    }
    sem_post(&Nempty);
    sleep(1);
  }

  pthread_exit(NULL);
}

int main(){


  pthread_t consumers[NUM_C_THREADS]; //thread array for consumers
  pthread_t producers[NUM_P_THREADS]; //thread array for producers

  //semaphores & mutex init
  pthread_mutex_init(&mutex, NULL);
  sem_init(&Nempty, 0, BUFFER_SIZE);
  sem_init(&Nfull, 0, 0);

  // Iterate and create producers
  for (size_t i = 0; i < NUM_P_THREADS; i++) {
    if(pthread_create(&producers[i], NULL, producer, (void*) i) < 0){
      std::cerr << "Something went wrong when creating the threads.\n";
      exit(-1);
    }
  }

  // Iterate and create consumers
  for (size_t i = 0; i < NUM_C_THREADS; i++) {
    if(pthread_create(&consumers[i], NULL, consumer, (void*) i) < 0){
      std::cerr << "Something went wrong when creating the threads.\n";
      exit(-1);
    }
  }

  // Terminate producer threads
  for (size_t i = 0; i < NUM_P_THREADS; i++) {
    if(pthread_join(producers[i], NULL) < 0){
      std::cerr << "Something went wrong when joining the threads.\n";
      exit(-1);
    }
  }

  // Terminate consumer threads
  for (size_t i = 0; i < NUM_C_THREADS; i++) {
    if(pthread_join(consumers[i], NULL) < 0){
      std::cerr << "Something went wrong when joining the threads.\n";
      exit(-1);
    }
  }
 //print a message at the end to say you are done
  std::cout << "All threads are done.\n";
  std::cout << "Resources cleaned up.\n";

  //destroy sum and mutx
  if(sem_destroy(&Nfull) < 0) {
    std::cerr << "Something went wrong when destroying the threads.\n";
    exit(-1);
  }

  if(sem_destroy(&Nempty) < 0) {
    std::cerr << "Something went wrong when destroying the threads.\n";
    exit(-1);
  }

  if(pthread_mutex_destroy(&mutex) != 0){
    std::cerr << "Something went wrong when destroying the threads.\n";
    exit(-1);
  }

  pthread_exit(NULL); // kill after all iterationas

  std::cout << "Resources cleaned up.\n";

  return 0;
}