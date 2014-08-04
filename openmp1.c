/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <omp.h>

int main(void) {
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    printf("Hello world from thread %d of %d\n", id, nthreads);
  }

  return 0;
}
