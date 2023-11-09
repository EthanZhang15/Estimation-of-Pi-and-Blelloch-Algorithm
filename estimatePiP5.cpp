#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */

#define NUM_THREADS 8  // Define the number of threads

double f(double x) {
  return sqrt(1-x*x);
}

std::atomic<double> pi{0.0};
int numPoints = 100000000;
double step = 2.0 / numPoints;

void add_to_pi(double bar) {
  auto current = pi.load();
  while (!pi.compare_exchange_weak(current, current + bar));
}

void* computeSemi(void* threadIdPtr) {
  int thread_id = *(int*) threadIdPtr;
  for (int i = thread_id; i < numPoints; i += NUM_THREADS) {
    double x = -1.0 + i * step;
    add_to_pi(f(x) * step);
  }
  pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
  uint64_t execTime;
  struct timespec tick, tock;

  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, computeSemi, &thread_ids[i]);
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &tick);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  //multiply by 2 for full circle
  add_to_pi(pi.load());

  clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
  execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;

  printf("Number of Threads: %d\n", NUM_THREADS);
  printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int)execTime);
  printf("%.20f\n", pi.load());
  return 0;
}
