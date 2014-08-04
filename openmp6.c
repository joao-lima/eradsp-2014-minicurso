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
  double *A, media = 0.0f;
  int N = MAX_N;
  double t0, t1, tdelta;
  int i;

  A = (double*)calloc(MAX_N, sizeof(double));
  srand(1313);
  for(i = 0; i < N; i++)
    A[i] = rand() / (double)RAND_MAX;

  t0 = omp_get_wtime();
#pragma omp parallel for reduction(+:media)
  for(i = 0; i < N; i++)
    media += A[i];

  media = media / N;

  t1 = omp_get_wtime();
  tdelta = t1 - t0;
  printf("Media de %d numeros em tempo: %.4f segundos.\n", N, tdelta);

  return 0;
}

