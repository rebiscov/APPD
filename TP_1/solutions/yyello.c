#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, numProcs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  printf("Yyello from the rank %d/%d\n", rank, numProcs);
  MPI_Finalize();
  return 0;
}
