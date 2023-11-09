#include <pthread.h> /*used in other parts of the assignment */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>  /* for uint64  */
#include <time.h>    /* for clock_gettime */
#include <atomic>    /*used in other parts of the assignment */

double f(double x) {
  return sqrt(1-x*x);
}

double semicircle = 0.0;

int main(int argc, char *argv[]) {
  uint64_t execTime; /*time in nanoseconds */
  struct timespec tick, tock;

  int h = 23;
  double step = 2.0/h;

  clock_gettime(CLOCK_MONOTONIC_RAW, &tick);
 
  double x = -1.0;
  for (int i = 0; i < h; i++) {
   semicircle = semicircle + step*f(x);  // Add to local sum
   x = x + step;  // next x
  }

  clock_gettime(CLOCK_MONOTONIC_RAW, &tock);
  execTime = 1000000000 * (tock.tv_sec - tick.tv_sec) + tock.tv_nsec - tick.tv_nsec;
  
  printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) execTime);
  printf("%.20f\n", semicircle);
  return 0;
}