#include <stdio.h>
#include "mpi.h"

// New scatter and gather implementations signatures for part 3

int MPI_ScatterSingleInt(
  const int *sendbuf,
  int *recvbuf,
  int root,
  MPI_Comm comm)
{
  // IMPLEMENT NEW SCATTER HERE!
  
  return MPI_SUCCESS;
}

int MPI_GatherSingleInt(
  const int *sendbuf,
  int *recvbuf,
  int root,
  MPI_Comm comm)
{
  // IMPLEMENT NEW GATHER HERE!

  return MPI_SUCCESS;
}
