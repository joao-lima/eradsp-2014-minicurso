/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_N	20000000

int main(void)
{
  int N = MAX_N;
  double t0, t1, tdelta;
  int *a, *b;
  int i;

  a = (int*)calloc(MAX_N, sizeof(int));
  b = (int*)calloc(MAX_N, sizeof(int));
  for(i = 0; i < N; i++){
    a[i] = b[i] = 1;
  }

  t0 = omp_get_wtime();
#pragma omp parallel
  {
    /* variaveis locais para cada thread */
    int id, i, nthreads, istart, iend;
    id = omp_get_thread_num();
    nthreads = omp_get_num_threads();
 
    if(id == 0)
      printf("Soma de %d numeros com %d threads\n", N, nthreads);
    
    istart = id * N / nthreads;
    iend = (id + 1) * N / nthreads;
   
    if( id == nthreads-1 )
      iend = N;
    for(i = istart; i < iend; i++)
      a[i] = a[i] + b[i];
  }
  t1 = omp_get_wtime();
  tdelta = t1 - t0;
  printf("Soma de %d numeros em tempo: %.4f segundos.\n", N, tdelta);

  return 0;
}

