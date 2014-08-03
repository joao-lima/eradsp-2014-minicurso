/*
 * Copyright (c) 2014 UFSM, UNIPAMPA.
 *
 * Authors: Joao Lima (UFSM) and Claudio Schepke (Unipampa).
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 100

int main( int argc, char** argv ) {
	int myrank, size;
	int i, somalocal = 0, somatotal;

	int vet[TAM]; // suponha inicializado no nó 0 (considerado raiz)

	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // Quem sou ?
	MPI_Comm_size(MPI_COMM_WORLD, &size); // Quantos somos ?

	if(myrank == 0)
		for(i = 0; i < TAM; i++)
			vet[i] = 1;

	MPI_Bcast(vet, TAM, MPI_INT, 0, MPI_COMM_WORLD);
	
	for( i = (TAM/size)*myrank ; i < (TAM/size) *(myrank+1) ; i++)
		somalocal += vet[i]; // Realiza as somas parciais
		
	MPI_Reduce(&somalocal, &somatotal, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if(myrank == 0)
		printf("Soma = %d\n", somatotal);
	
	return 0;
}
