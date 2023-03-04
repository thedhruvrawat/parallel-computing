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
#include <math.h> 

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
   int iter_count = log2(comm_sz);
   for(int itr=0; itr<iter_count; itr++) {
        int active = comm_sz/pow(2, itr);
        int reciever = pow(2, iter_count-itr-1);
        if(my_rank%active == 0) {
            int sendrank = (my_rank + (comm_sz/(pow(2, (itr+1)))));  
            sprintf(greeting, "message from proc %d of %d!", my_rank, comm_sz);
            if(sendrank<comm_sz)
                MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, sendrank, 0, MPI_COMM_WORLD);
        } else if(my_rank%reciever == 0){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            printf("Iteration %d: Proc %d receives %s\n", itr+1, my_rank, greeting);
        }
   }

    
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
