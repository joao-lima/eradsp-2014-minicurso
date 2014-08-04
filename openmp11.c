/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <omp.h>

long fib (int n)
{
  long x, y;
  if (n < 2)
    return n;

#pragma omp taskgroup 
{
  #pragma omp task untied shared(x) firstprivate(n) depend(in:n) depend(out:x)
    x = fib(n - 1);
  #pragma omp task untied shared(y) firstprivate(n) depend(in:n) depend(out:y)
    y = fib(n - 2);
}

  return x + y;
}

int main(void) {
  int n = 20;
  long res = 0;
  double t0, t1, tdelta;

  t0 = omp_get_wtime();

#pragma omp parallel
#pragma omp single
  res = fib(n);
  
  t1 = omp_get_wtime();
  tdelta = t1 - t0;
  printf("Fibonacci de %d e %ld em tempo %0.4f segundos.\n", n, res, tdelta);
  
  return 0;
}
