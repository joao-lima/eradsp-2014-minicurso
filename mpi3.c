/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv){
	int rank,size,tag,i;
	MPI_Status status;
	char msg[20];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(rank == 0) {
		strcpy(msg, "Hello World!\n");
		for(i=1; i < size; i++)
			MPI_Send(msg, 13, MPI_CHAR, i, tag, MPI_COMM_WORLD);
	} else {
		MPI_Recv(msg, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		printf("Process %d: Message received: %s\n", rank, msg);
	}
	MPI_Finalize();
	return 0;
}
