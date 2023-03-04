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
   char       greetingA[MAX_STRING], greetingB[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */

   /* Start up MPI */
   MPI_Init(NULL, NULL); 

   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank != 0) { 
      /* Create message */
      sprintf(greetingA, "[Tag A] Greetings from process %d of %d!", 
            my_rank, comm_sz);
      /* Send message with tag A */
      MPI_Send(greetingA, strlen(greetingA)+1, MPI_CHAR, 0, 0,
            MPI_COMM_WORLD); 

      sprintf(greetingB, "[Tag B] Greetings from process %d of %d!", 
            my_rank, comm_sz);
      /* Send message with tag B */
      MPI_Send(greetingB, strlen(greetingB)+1, MPI_CHAR, 0, 1,
            MPI_COMM_WORLD); 
   } else {  
      /* Print my message */
      printf("Greetings from process %d of %d!\n", my_rank, comm_sz);
      MPI_Status status;
      for (int q = 1; q < comm_sz; q++) {
         /* Receive message from any process */
         MPI_Recv(greetingB, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE,
            1, MPI_COMM_WORLD, &status);
        int source = status.MPI_SOURCE;
         printf("[Proc: %d Tag: B] %s\n", source, greetingB);
         MPI_Recv(greetingA, MAX_STRING, MPI_CHAR, source,
            0, MPI_COMM_WORLD, &status);
         printf("[Proc: %d Tag: A] %s\n", source, greetingA);
         
      } 
   }

   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
