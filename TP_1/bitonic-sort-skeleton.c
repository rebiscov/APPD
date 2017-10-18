#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int compareInt(
    const void *p1,
    const void *p2)
{
  return *((int *)p1) - *((int *)p2);
}

void readArray(
    int *arr,
    int N,
    char *fileName)
{
  FILE *file = fopen(fileName, "r");
  if (!file) { 
    printf("ERROR: Unable to open the file %s.\n", fileName);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  for (int i = 0; i < N; i++) {
    fscanf(file, " %d", arr + i);
  }

  fclose(file);
}

void printArray(
    int *arr,
    int N)
{
  for (int i = 0; i < N; i++) { printf("%d ", arr[i]); }
  printf("\n");
}

int checkSorted(
    int *arr,
    int N)
{
  for (int i = 0; i < N - 1; i++) {
    if (arr[i] > arr[i + 1]) { return 0; }
  }
  return 1;
}

void printUsage()
{
  printf("Usage: mpirun -np [num-procs/elements] ./bitonic-sort [bitonic-array-file] [sequential|parallel]\n");
}

int main(int argc, char **argv)
{
  int *arr = NULL;  // The array of elements to sort. It exists only in the master rank, and is NULL in other ranks.
  int N;            // The size of arr. Equally, the number of available MPI ranks.
  int procElem;     // The element assigned to the current process in bitonic sort.
  int procRank;     // The rank of current process
  int numProcs;     // The number of processes. Same as N.

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  N = numProcs;

  if (argc < 3 && procRank == 0) {
    printUsage();
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  arr = (int *)malloc(numProcs * sizeof(arr[0]));
  if (procRank == 0) { 
    readArray(arr, N, argv[1]);
    printf("->The array before bitonic sort:\n");
    printArray(arr, N);
  }
  
  if (strcmp(argv[2], "sequential") == 0) {
    if (procRank == 0) {
      qsort(arr, N, sizeof(arr[0]), compareInt);
    }
  } 
  else if (strcmp(argv[2], "parallel") != 0) {
    if (procRank == 0) {
      printf("ERROR: Unknown array sort type %s.\n", argv[2]);
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
#include "bitonic-sort-solution.c"
  MPI_Barrier(MPI_COMM_WORLD);

  if (procRank == 0) {
    printf("->The array after bitonic sort:\n");
    printArray(arr, N);
    if (checkSorted(arr, N)) {
      printf("->The array is sorted correctly!\n");
    } else {
      printf("->The array does not seem to be sorted!\n");
    }
  }

  MPI_Finalize();
  return 0;
}
