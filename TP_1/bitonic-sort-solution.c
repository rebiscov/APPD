// int *arr;       // The array of elements to sort. It exists only in the master rank, and is NULL in other ranks.
// int N;          // The number of elements, and the size of arr. Equally, the number of available MPI ranks.
// int procElem;   // The element assigned to the current process in bitonic sort.
// int procRank;   // The rank of current process
// int numProcs;   // The number of processes. Same as N.

// New scatter and gather signatures for the question 3
#include <math.h>

int MPI_ScatterSingleInt(
  const int *sendbuf,
  void *recvbuf,
  int root,
  MPI_Comm comm);

int MPI_GatherSingleInt(
  const int *sendbuf,
  void *recvbuf,
  int root,
  MPI_Comm comm);

// IMPLEMENT PARALLEL BITONIC SORT HERE!

MPI_Scatter((const void *)arr, 1, MPI_INT, (void *)&procElem, 1, MPI_INT, 0, MPI_COMM_WORLD);
int temp;

for (int i = 0; i < (int)log(N); i++) {
  int n = N/(int)pow(2,i);
  
  if (procRank % 2 == 0){
    int rcv;
    MPI_Recv((void *)&rcv, 1, MPI_INT, procRank+n/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    if (procElem > rcv)
      MPI_Send((void *)&rcv, 1, MPI_INT, procRank+n/2, 0, MPI_COMM_WORLD);
    else{
      temp = procElem;
      MPI_Send((void *)&temp, 1, MPI_INT, procRank+n/2, 0, MPI_COMM_WORLD);
      procElem = rcv;
    }
  }
  else{
    MPI_Send((void *)&procElem, 1, MPI_INT, procRank-n/2, 0, MPI_COMM_WORLD);
    MPI_Recv((void *)&procElem, 1, MPI_INT, procRank-n/2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
}



