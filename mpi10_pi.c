/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 *
 * Based on Tim Mattson's example of Pi 11/99.
 */

#include <stdio.h>
#include <mpi.h>

static long num_steps = 100000000;
double step;

int main (int argc, char** argv)
{
  int i, rank, size;
  double t0, t1, tdelta;
  double pi, sum = 0.0, total_sum;
  
  step = 1.0/(double) num_steps;
  
  double x;
  
  /* Inicia o MPI */
  MPI_Init(&argc, &argv);
  
  /* Identifica e o quantos processos rodam */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  if(rank == 0){
    printf("Calculo do Pi com %ld iteracoes e %d processos\n", num_steps, size);
  }

  t0 = MPI_Wtime();

  for (i= rank; i < num_steps; i += size){
    x = (i+0.5)*step;
    sum = sum + 4.0/(1.0+x*x);
  }
 
  MPI_Reduce(&sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
  t1 = MPI_Wtime();

  if(rank == 0){
    pi = step * total_sum;
    tdelta = t1 - t0;
    printf("\n Pi e %f em %f segundos com %d processos\n", pi, tdelta, size);
  }

  MPI_Finalize();
  return 0;
}





