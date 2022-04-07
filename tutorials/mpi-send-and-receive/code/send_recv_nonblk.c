// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// to process 1.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void test_fucn1 (){
    int i =3;

}
void test_fucn2 (){
    int j =34;
}

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int number [100000] = {11, 22, 33, 44};
  number [99999] = 999;
  int dest [100000];
  MPI_Request send_req;
  MPI_Request recv_req;
  MPI_Status stat;

  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    MPI_Isend(
      /* data         = */ &number, 
      /* count        = */ 100000, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD,
      /* request      = */ &send_req);
    printf("sender waiting...");
    MPI_Wait(&send_req, &stat);
  } else if (world_rank == 1) {
    MPI_Irecv(
      /* data         = */ &dest, 
      /* count        = */ 100000, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* request      = */ &recv_req);
    printf("receiver waiting...");
    MPI_Wait(&recv_req, &stat);
    printf("Process 1 received number %d from process 0\n", dest[0]);
    printf("Process 1 received number %d from process 0\n", dest[1]); printf("Process 1 received number %d from process 0\n", dest[2]);
    printf("Process 1 received number %d from process 0\n", dest[3]);
    printf("Process 1 received number %d from process 0\n", dest[99999]);
  }
  MPI_Finalize();
}

