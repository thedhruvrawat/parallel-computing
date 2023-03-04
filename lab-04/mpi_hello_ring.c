/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -std=C99 -o mpi_hello mpi_hello.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_hello
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   MPI_Status status;
//    int MAX_SIZE = comm_sz; 


   if (my_rank != 0) { 
      /* Create message */
      sprintf(greeting, "Greetings from left neighbour process %d of %d!", my_rank, comm_sz);
      /* Send message with tag A */
      MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, (my_rank+1)%comm_sz, 0, MPI_COMM_WORLD); 

      MPI_Recv(greeting, MAX_STRING, MPI_CHAR, my_rank-1, 0, MPI_COMM_WORLD, &status);

      printf("[%d] %s\n", my_rank, greeting);
      
   } else {  
         /* Receive message from any process */
        // MPI_Recv(greeting, MAX_STRING, MPI_CHAR, (my_rank-1)%MAX_SIZE, 0, MPI_COMM_WORLD, &status);

        // printf("[%d] %s\n", status.MPI_SOURCE, greeting);
        /* Create message */
        sprintf(greeting, "Greetings from left neighbour process %d of %d!", my_rank, comm_sz);
        /* Send message with tag A */
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD); 
        
        MPI_Recv(greeting, MAX_STRING, MPI_CHAR, comm_sz-1, 0, MPI_COMM_WORLD, &status);

        printf("[%d] %s\n", my_rank, greeting);
   }

    
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
