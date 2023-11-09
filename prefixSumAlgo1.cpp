#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <random>

#define NUM_THREADS 8  // Define the number of threads
#define ARRAY_SIZE 2000000  // Define the Array Size

double arr[ARRAY_SIZE];
int segmentSize = ARRAY_SIZE/NUM_THREADS;
pthread_barrier_t barrier;

//print array to check values are correct
void printArr(){
  printf("[%.1lf", arr[0]);
  for (int i = 1; i < ARRAY_SIZE; i++) {
      printf(", %.1lf", arr[i]);
  }
  printf("]\n");
  return;
}

//method for each thread
void* segmentPrefixSum(void* threadIdPtr) {
  //computes each segment
  int thread_id = *(int*) threadIdPtr;
  int offset = thread_id * segmentSize;
  for (int i = 1; i < segmentSize; i ++) {
    arr[i + offset] += arr[i + offset - 1];
  }
  pthread_barrier_wait(&barrier);
  //adds each segment over to the next
  int curSegmentI = segmentSize;
  while(curSegmentI < ARRAY_SIZE){
    for (int i = thread_id; i < segmentSize; i += NUM_THREADS) {
      arr[i + curSegmentI] += arr[curSegmentI - 1];
    }
    curSegmentI+=segmentSize;
    pthread_barrier_wait(&barrier);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  uint64_t execTime;
  struct timespec tick, tock;

  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];
  pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);

  //intialize array
  srand(time(NULL));
  for (int i = 0; i < ARRAY_SIZE; i++){
    arr[i] = rand()%11;
  }

  //initialize threads
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, segmentPrefixSum, &thread_ids[i]);
  }

  //start timer
  clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

  pthread_barrier_wait(&barrier);

  //Sync main thread with worker threads
  for(int x = 1; x < NUM_THREADS; x++){
    pthread_barrier_wait(&barrier);
  }

  //stop timer
  clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
  execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

  printf("Number of Threads: %d\n", NUM_THREADS);
  printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int)execTime);
  return 0;
}