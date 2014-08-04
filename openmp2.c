/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <omp.h>

int main(void) {
  omp_set_num_threads(4); 
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    printf("Thread ID %d\n", id);
  }
  printf("Parte sequencial ...\n");
  #pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    printf("Thread ID %d\n", id);
  }

  return 0;
}
