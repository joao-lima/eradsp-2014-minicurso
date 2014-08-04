/*
 * History: Written by Tim Mattson, 11/99.
 * Modified by Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <omp.h>

static long	num_steps = 100000000;
double		step;
int
main()
{
  int		i;
  double		x      , pi, sum = 0.0;
  double		start_time, run_time;

  step = 1.0 / (double)num_steps;
  start_time = omp_get_wtime();

#pragma omp parallel  
  {
    #pragma omp single
      printf("Pi com num_threads = %d\n", omp_get_num_threads());

#pragma omp for private(x) reduction(+:sum)
    for (i = 1; i <= num_steps; i++) {
      x = (i - 0.5) * step;
      sum = sum + 4.0 / (1.0 + x * x);
    }
  }

  pi = step * sum;
  run_time = omp_get_wtime() - start_time;
  printf("Pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
