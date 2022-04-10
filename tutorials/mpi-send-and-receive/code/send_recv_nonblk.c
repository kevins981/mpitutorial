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

void test_func1 (){
    int i =3;
    printf("test func 1");

}
void test_func2 (int * arr){
    for (int i = 0; i < 5; i++) {
        arr[i] = arr[0] + arr[i+1];
    }
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

  test_func1();
  int test_arr [10] = {123,456,789,1,2,3,4,5,6};
  srand (0);
  int number [100000] = {11, 22, 33, 44};
  number [99999] = 999;
  int dest [100000] = {1,2,3,4,5,6,7,8,9,10};
  MPI_Request send_req;
  MPI_Request recv_req;
  MPI_Status stat;

  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    MPI_Isend(
      /* data         = */ number, 
      /* count        = */ 100000, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD,
      /* request      = */ &send_req);
    printf("sender waiting...\n");
    MPI_Wait(&send_req, &stat);
    // need to handle the below case. Cannot move
    // https://stackoverflow.com/questions/71066473/in-llvm-how-do-i-get-the-loop-and-its-instructions
    //for (int i = 0; i < 5; i++) {
    //    number[i] = number[0] + number[i+1];
    //}
    // should hoist the below loop
    for (int i = 0; i < 10; i++) {
        test_arr[i] = test_arr[i] + test_arr[i+1] + rand() % 10 ;
    }
    // TODO: test if we can hoist this. We should be able to
    int test = number[0] + rand() % 10;
    // should not hoist the below line
    number[0] = 10*3;
    // print compute results
    for (int i = 0; i < 10; i++) {
        printf("Source: print test arr %d \n", test_arr[i]);
    }
  } else if (world_rank == 1) {
    MPI_Irecv(
      /* data         = */ dest, 
      /* count        = */ 100000, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* request      = */ &recv_req);
    printf("receiver waiting...");
    MPI_Wait(&recv_req, &stat);
    // should be able to hoist the below loop
    for (int i = 0; i < 10; i++) {
        test_arr[i] = test_arr[i] + test_arr[i+1] + rand() % 9 ;
    }
    // should not be able to hoist the line below
    int test = dest[0] + rand() % 10;
    printf("Test is %d\n", test);
    // should not be able to hoist the line below
    dest[1] = 2 + rand() % 10;
    printf("Process 1 received number %d from process 0\n", dest[0]);
    for (int i = 0; i < 10; i++) {
        printf("Dest: print test arr %d \n", test_arr[i]);
    }
  }
  MPI_Finalize();
}

