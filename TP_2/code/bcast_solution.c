// Already defined variables in the original source code. Do not uncomment any of those, just use them directly!
// Process rank 0 should be the source of the broadcast
//
// int num_procs;
// int rank;
// char *bcast_implementation_name:   the bcast implementation name (argument #1)
// int chunk_size:                    the chunk size (optional argument #2)
// int NUM_BYTES:                     the number of bytes to broadcast
// char *buffer:                      the buffer to broadcast
//
// The method names should be:
// default_bcast
// naive_bcast
// ring_bcast
// pipelined_ring_bcast
// asynchronous_pipelined_ring_bcast
// asynchronous_pipelined_bintree_bcast
//
// GOOD LUCK (gonna need it)!

if (strcmp(bcast_implementation_name, "default_bcast") == 0) { // Just calling the library routine.
  MPI_Bcast(buffer, NUM_BYTES, MPI_CHAR, 0, MPI_COMM_WORLD);
 } else if (strcmp(bcast_implementation_name, "naive_bcast") == 0) { // Send to all processes one-by-one from the root.
  if (rank == 0)
    for (int i = 0; i < num_procs; i++)
      MPI_Send(buffer, NUM_BYTES, MPI_CHAR, i+1, 0, MPI_COMM_WORLD);
  else
    MPI_Recv(buffer, NUM_BYTES, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 } else if (strcmp(bcast_implementation_name, "ring_bcast") == 0){
  if (rank == 0)
    MPI_Send(buffer, NUM_BYTES, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
  else if (rank == num_procs-1)
    MPI_Recv(buffer, NUM_BYTES, MPI_CHAR, num_procs-2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  else{
    MPI_Recv(buffer, NUM_BYTES, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(buffer, NUM_BYTES, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
  }
 } else if (strcmp(bcast_implementation_name, "pipelined_ring_bcast") == 0){
  for (int r = 0; r < NUM_BYTES / chunk_size; r++){
    if (rank == 0)
      MPI_Send(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    else if (rank == num_procs-1)
      MPI_Recv(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    else{
      MPI_Recv(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
    }
  }
 } else if (strcmp(bcast_implementation_name, "asynchronous_pipelined_ring_bcast") == 0){
  MPI_Request req;
  
  for (int r = 0; r < NUM_BYTES / chunk_size; r++){
    if (rank == 0){
      MPI_Wait(&req, MPI_STATUS_IGNORE);
      MPI_Isend(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &req);
    }
    else if (rank == num_procs-1)
      MPI_Recv(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    else{
      MPI_Recv(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Wait(&req, MPI_STATUS_IGNORE);      
      MPI_Isend(buffer + r*chunk_size, chunk_size, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD, &req);
    }
  }
 }




