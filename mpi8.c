/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void printvector(int rank, int *buffer) {
	int size, i;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	for (i = 0; i < size; i++)
		printf("rank %d - %d\n", rank, buffer[i]);
}

int main(int argc, char **argv) {
	int *sndbuffer, *recvbuffer;
	int rank, size, i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	sndbuffer = (int *)malloc(size*sizeof(int));
	recvbuffer = (int *)malloc(size*sizeof(int));

	for(i=0; i<size; i++) sndbuffer[i] = i*i+rank;
	printvector(rank, sndbuffer);

	MPI_Alltoall(sndbuffer, 1, MPI_INT, recvbuffer, 1, MPI_INT, MPI_COMM_WORLD);
	printvector(rank, recvbuffer);

	MPI_Finalize();
	return 0;
}

