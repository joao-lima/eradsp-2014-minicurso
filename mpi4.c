/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <mpi.h>
int main(int argc, char **argv) {
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("I'm %d of %d\n", rank, size);

	if(rank == 0) {
		printf("(%d) -> Primeiro a escrever!\n", rank);
		MPI_Barrier(MPI_COMM_WORLD);
	} else {
		MPI_Barrier(MPI_COMM_WORLD);
		printf("(%d) -> Agora posso escrever!\n", rank);
	}

	MPI_Finalize();
	return 0;
}
