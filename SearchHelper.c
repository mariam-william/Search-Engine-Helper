#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <limits.h>
#include <stdbool.h>

char query[350];
int nQueries, totalQueries = 0;
int searchQuery(int, int, int);


int main(int argc, char* argv[]) {

	int rank, nProcesses;
	int source, dest, tag = 0;
	int i, process_Size = 50, remProcess_Size, nFiles = 50, nLines = 30, sentenceSize = 350;
	MPI_Status status;
	MPI_Comm world = MPI_COMM_WORLD;
	double startTime, endTime;

	/* Start up MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(world, &rank);
	MPI_Comm_size(world, &nProcesses);

	startTime = MPI_Wtime();

	if (nProcesses == 1) {	// Handling non-parallelialism
		printf("Hello, this is Search Engine Helper\n************************\n Please enter your query: \n");
		fgets(query, 350, stdin);
		int idx = strlen(query) - 1;
		query[idx] = '\0';
		nQueries = searchQuery(1, nFiles, rank);
		printf("Rank %d result is: %d\n", rank, nQueries);
		printf("\nTotal Queries = %d\n", nQueries);
		
		FILE* fp1;
		char filename[50];
		sprintf(filename, "Total_Queries_Result.txt");
		fp1 = fopen(filename, "w");
		fprintf(fp1, "Query: %s \n", query);
		fprintf(fp1, "Search Results Found = %d\n", nQueries);

		endTime = MPI_Wtime();
		printf("\nProcessing time = %lf seconds\n", endTime - startTime);
	}

	else { // Parallel program case
		process_Size = nFiles / (nProcesses - 1);
		remProcess_Size = nFiles % (nProcesses - 1);

		if (rank == 0) {	// Master's work

			printf("Hello, this is Search Engine Helper\n************************\n Please enter your query: \n");
			fgets(query, 350, stdin);
			int idx = strlen(query) - 1;
			query[idx] = '\0';

			int start = 1, end = 0;
			//scatter work to slaves
			for (i = 1; i < nProcesses; i++)
			{
				end += (process_Size);
				if (i == nProcesses - 1)
					end = 50;
				MPI_Send(&start, 1, MPI_INT, i, 0, world);
				MPI_Send(&end, 1, MPI_INT, i, 0, world);
				start += process_Size;
			}
			// Handling Remainder in Master
			if (remProcess_Size > 0) {
				//printf("Rem: %d\n", remProcess_Size);
				start = nFiles - (remProcess_Size + 1);
				nQueries = searchQuery(start, nFiles, rank);
				printf("Rank %d result is: %d\n", rank, nQueries);
			}
		}

		MPI_Bcast(&query, 350, MPI_CHAR, 0, world);	//broadcasting the search query to all slaves

		if (rank != 0) {	//Slave work
			int start, end;
			MPI_Recv(&start, 1, MPI_INT, 0, 0, world, &status);
			MPI_Recv(&end, 1, MPI_INT, 0, 0, world, &status);

			nQueries = searchQuery(start, end, rank);
			printf("Rank %d result is: %d\n", rank, nQueries);
		}

		// Reducing queries result count
		MPI_Reduce(&nQueries, &totalQueries, 1, MPI_INT, MPI_SUM, 0, world);

		if (rank == 0) {	//Storing queries count
			printf("\nTotal Queries = %d\n", totalQueries);
			FILE* fp1;
			char filename[50];

			sprintf(filename, "Total_Queries_Result.txt");
			fp1 = fopen(filename, "w");
			fprintf(fp1, "Query: %s \n", query);
			fprintf(fp1, "Search Results Found = %d\n", totalQueries);

			endTime = MPI_Wtime();
			printf("\nProcessing time = %lf seconds\n", endTime - startTime);
		}
	}

	MPI_Finalize();
	return 0;
}


int searchQuery(int start, int end, int my_rank) {
	int i, foundCount = 0;
	int found = -1;
	FILE* fp = NULL, * fp2 = NULL;
	char line[350];
	char* word;
	char filename[50];
	char delim[] = " ";
	char newQ[350];

	sprintf(filename, "Result-From-Rank_%d.txt", my_rank);
	fp2 = fopen(filename, "w");
	fprintf(fp2, "Query: %s \n\n", query);

	//Read from each file line by line

	for (i = start; i < end; i++) {
		sprintf(filename, "Aristo-Mini-Corpus/Aristo-Mini-Corpus P-%d.txt", i);
		fp = fopen(filename, "r");

		if (fp == NULL) {
			printf("failed reading data\n");
			return;
		}
		int count = 0;
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			strcpy(newQ, query);
			found = 1;
			char* tempQuery = strtok(newQ, delim);
			while (tempQuery != NULL) {
				char tempLine[350];
				strcpy(tempLine, line);
				word = strstr(tempLine, tempQuery);
				if (!word) {
					found = -1;
					break;
				}
				found = 1;
				tempQuery = strtok(NULL, delim);
			}
			if (found == 1) {
				fputs(line, fp2);
				foundCount++;
			}
		}
	}
	fprintf(fp2, "\nSearch Results found = %d\n", foundCount);
	fclose(fp);
	fclose(fp2);
	return foundCount;
}
