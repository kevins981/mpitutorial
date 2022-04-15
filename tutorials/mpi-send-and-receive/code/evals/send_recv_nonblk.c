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
#include <unistd.h>
#include <time.h>
#define BILLION 1000000000L

//inline __attribute__((always_inline)) int compute(){
//  //size_t COMPUTE_SIZE = 100000000;
//  size_t COMPUTE_SIZE = 100000;
//  //int * arr = (int*)malloc(COMPUTE_SIZE * sizeof(int));
//  //for (int i = 0; i < COMPUTE_SIZE; i++) {
//  //  arr[i] = rand();
//  //}
//  int sum = 0;
//  #pragma unroll
//  for (int i = 0; i < COMPUTE_SIZE; i++) {
//    sum += (rand() * (rand()+5) / (i+1) + 11234) / (rand()+2) ;
//  }
//
//  return sum;
//
//}

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

  srand (0);
  long SIZE = 1000000000;
  int * src = (int*)malloc(SIZE * sizeof(int));
  int * dest = (int*)malloc(SIZE * sizeof(int));
  MPI_Request send_req;
  MPI_Request recv_req;
  MPI_Status stat;

  u_int64_t diff;
  struct timespec start, end;
  //for (int i = 0; i < SIZE; i++) {
  //  src[i] = rand()%1000;
  //}

  if (world_rank == 0) {
    clock_gettime(CLOCK_MONOTONIC, &start);	/* mark start time */

    MPI_Isend(
      /* data         = */ src, 
      /* count        = */ SIZE, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD,
      /* request      = */ &send_req);

    /////// computer block start
    //size_t COMPUTE_SIZE = 1000;
    int sum = 0;

    //#pragma unroll
    //for (int i = 0; i < COMPUTE_SIZE; i++) {
    //  sum += rand() * (rand()+5) / (i+1);
    //}

    /////// computer block end
    MPI_Wait(&send_req, &stat);
    //sum = compute();
    sleep(3);

	clock_gettime(CLOCK_MONOTONIC, &end);	/* mark the end time */
	diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
	printf("elapsed time = %llu us\n", (long long unsigned int) diff/1000);

  } else if (world_rank == 1) {

    MPI_Irecv(
      /* data         = */ dest, 
      /* count        = */ SIZE, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* request      = */ &recv_req);

    MPI_Wait(&recv_req, &stat);

    //for (long i = 0; i < 10; i++) {
    //    printf("Received [%d]: %d \n", i, dest[i]);
    //}
    //for (long i = SIZE-10; i < SIZE; i++) {
    //    printf("Received [%d]: %d \n", i, dest[i]);
    //}
  }
  MPI_Finalize();
}

