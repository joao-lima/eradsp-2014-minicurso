/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char **argv) {
	char *message = (char *) malloc(sizeof(char) * 30);
	int rank, size, length;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("I'm %d of %d\n",rank,size);

	if(rank == 0) {
		strcpy(message, "Hello_World!");
		length = strlen(message);
		printf("%d\n", length);
	}

	MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(message, length + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

	if(rank != 0)
		printf("(%d) – Received %s\n", rank, message);

	MPI_Finalize();
	return 0;
}
