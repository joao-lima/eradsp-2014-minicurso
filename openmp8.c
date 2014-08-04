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
  double	pi, full_sum = 0.0;
  double		start_time, run_time;

  step = 1.0 / (double)num_steps;
  start_time = omp_get_wtime();

#pragma omp parallel private(i)
  {
    int id = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    double x;
    double sum = 0.0f;

    #pragma omp single
      printf("Pi com num_threads = %d\n", nthreads);

    for (i = id; i < num_steps; i+= nthreads) {
      x = (i - 0.5) * step;
      sum = sum + 4.0 / (1.0 + x * x);
    }

  /* apenas uma thread soma por vez o resultado final */
#pragma omp critical
    full_sum += sum;
  }

  pi = step * full_sum;
  run_time = omp_get_wtime() - start_time;
  printf("Pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
