/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <mpi.h>

#ifndef NBTASKS
#define NBTASKS 10
#endif

#ifndef ITER
#define ITER 100000
#endif

#define END -1

#define DEBUG
#ifdef DEBUG
#define debug(FMT, ...) fprintf(stdout, FMT, ##__VA_ARGS__)
#else
#define debug(FMT, ...)
#endif

int tag = 110;
int tag_res = 111;

double calcula (int n)
{
  double resultado = 0.0f;
  int i;
  
  for (i = 0; i < ITER; i++)
    resultado = sqrt ((double) i + n * resultado);
  return (resultado);
}

void mestre ()
{
  int task = 0, sinal, nb_slaves, recebido;
  double res, res2, total_res = 0.0;
  MPI_Status stat;
  MPI_Request recv_request;
  
  MPI_Comm_size (MPI_COMM_WORLD, &nb_slaves);
  nb_slaves--;
  /* Primeiro, manda uma tarefa para cada escravo */
  for (task = 0; task < nb_slaves; task++) {
    debug ("Mestre manda tarefa %d para o escravo %d\n", task, task + 1);
    MPI_Send (&task, 1, MPI_INT, task + 1, tag, MPI_COMM_WORLD);
  }
  
  /* Dispara um primeiro recv nao-bloqueante */
  MPI_Irecv (&res, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_res,
             MPI_COMM_WORLD, &recv_request);
  while (task < NBTASKS) {
    /* Testa se um Irecv tem sido efetuado */
    MPI_Test (&recv_request, &recebido, &stat);

    if (recebido) {
      /* Manda ja mais algo para fazer */
      debug ("Mestre manda tarefa %d para o escravo %d\n", task,
             stat.MPI_SOURCE);
      total_res += res;
      MPI_Send (&task, 1, MPI_INT, stat.MPI_SOURCE, tag, MPI_COMM_WORLD);
      MPI_Irecv (&res, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_res,
                 MPI_COMM_WORLD, &recv_request);
    } else {
      debug ("Mestre calcula tarefa %d...\n", task);
      total_res += calcula (task);
    }
    
    task++;
  }
  
  /* Sobram resultados para receber */
  for (task = 0; task < nb_slaves - 1; task++) {
    /* a variavel "res" nao pode ser usada enquanto tiver um Irecv sobre ela. */
    MPI_Recv (&res2, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag_res, MPI_COMM_WORLD,
              &stat);
    total_res += res2;
    /* Manda uma mensagem de finalizacao */
    sinal = END;
    MPI_Send (&sinal, 1, MPI_INT, stat.MPI_SOURCE, tag, MPI_COMM_WORLD);
  }
  /* Ainda tem um Irecv em andamento  - espera ate o fim dele */
  MPI_Wait (&recv_request, &stat);
  /* Agora acabou, sinaliza para quem estava mandando que terminou tudo. */
  debug ("Mestre recebeu o ultimo irecv de %d e lhe sinaliza o fim.\n", stat.MPI_SOURCE);
  total_res += res;
  sinal = END;
  MPI_Send (&sinal, 1, MPI_INT, stat.MPI_SOURCE, tag, MPI_COMM_WORLD);
  
  fprintf (stdout, "Resultado total: %0.4f\n", total_res);
}

/* Nada mudou nele */
void escravo ()
{
  int task = 0, rank;
  double res;
  MPI_Status stat;
  
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  do {
    MPI_Recv (&task, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &stat);
    if (task == END)
      break;
    debug ("Escravo %d: processando tarefa %d.\n", rank, task);
    res = calcula (task);
    MPI_Send (&res, 1, MPI_DOUBLE, 0, tag_res, MPI_COMM_WORLD);
  }
  while (1);
  debug ("Escravo %d acabando...\n", rank);
}

int
main (int argc, char *argv[])
{
  int size, rank;
  double t1, t2, tdelta;
  
  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);
  
  t1 = MPI_Wtime ();
  if (rank == 0) {
    debug ("Processo 0 e o mestre.\n");
    mestre ();
  } else {
    debug ("Process %d e um escravo.\n", rank);
    escravo ();
  }
  t2 = MPI_Wtime ();
  
  if (rank == 0) {
    tdelta = t2 - t1;
    fprintf (stdout, "%d processos com tempo %0.2f segundos.\n", size, tdelta);
  }
  
  MPI_Finalize ();
  
  return 0;
}
