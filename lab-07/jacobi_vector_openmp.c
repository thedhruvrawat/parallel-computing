#include <stdio.h>
#include <math.h>
#include "mpi.h"

/* This example handles a 1024 x 1024 mesh, on 4 processors only. */
#define maxn 1024

int main( argc, argv )
int argc;
char **argv;
{
    int        rank, value, size, errcnt, toterr, i, j, itcnt;
    int        i_first, i_last;
    MPI_Status status;
    double     diffnorm, gdiffnorm;
    double     xlocal[(1024/4)+2][1024];
    double     xnew[(1024/3)+2][1024];

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    if (size != 4) MPI_Abort( MPI_COMM_WORLD, 1 );

    /* xlocal[][0] is lower ghostpoints, xlocal[][maxn+2] is upper */

    /* Note that top and bottom processes have one less row of interior
       points */
    i_first = 1;
    i_last  = maxn/size;
    if (rank == 0)        i_first++;
    if (rank == size - 1) i_last--;

    /* Fill the data as specified */
	#  pragma omp parallel for num_threads(4) 
    for (i=1; i<=maxn/size; i++) 
	for (j=0; j<maxn; j++) 
	    xlocal[i][j] = rank;
    #  pragma omp parallel for num_threads(4) 
	for (j=0; j<maxn; j++) {
	xlocal[i_first-1][j] = -1;
	xlocal[i_last+1][j] = -1;
    }

    itcnt = 0;
    do {
	/* Send up unless I'm at the top, then receive from below */
	/* Note the use of xlocal[i] for &xlocal[i][0] */
	if (rank < size - 1) 
	    MPI_Send( xlocal[maxn/size], maxn, MPI_DOUBLE, rank + 1, 0, 
		      MPI_COMM_WORLD );
	if (rank > 0)
	    MPI_Recv( xlocal[0], maxn, MPI_DOUBLE, rank - 1, 0, 
		      MPI_COMM_WORLD, &status );
	/* Send down unless I'm at the bottom */
	if (rank > 0) 
	    MPI_Send( xlocal[1], maxn, MPI_DOUBLE, rank - 1, 1, 
		      MPI_COMM_WORLD );
	if (rank < size - 1) 
	    MPI_Recv( xlocal[maxn/size+1], maxn, MPI_DOUBLE, rank + 1, 1, 
		      MPI_COMM_WORLD, &status );
	
	/* Compute new values (but not on boundary) */
	itcnt ++;
	diffnorm = 0.0;
	#  pragma omp parallel for num_threads(4) 
	for (i=i_first; i<=i_last; i++) 
	    for (j=1; j<maxn-1; j++) {
		xnew[i][j] = (xlocal[i][j+1] + xlocal[i][j-1] +
			      xlocal[i+1][j] + xlocal[i-1][j]) / 4.0;
		diffnorm += (xnew[i][j] - xlocal[i][j]) * 
		            (xnew[i][j] - xlocal[i][j]);
	    }
	/* Only transfer the interior points */
	#  pragma omp parallel for num_threads(4) 
	for (i=i_first; i<=i_last; i++) 
	    for (j=1; j<maxn-1; j++) 
		xlocal[i][j] = xnew[i][j];

	// #pragma omp barrier

	MPI_Allreduce( &diffnorm, &gdiffnorm, 1, MPI_DOUBLE, MPI_SUM,
		       MPI_COMM_WORLD );
	gdiffnorm = sqrt( gdiffnorm );
	// if (rank == 0) printf( "At iteration %d, diff is %e\n", itcnt, gdiffnorm );
    } while (gdiffnorm > 1.0e-2 && itcnt < 100);

    MPI_Finalize( );
    return 0;
}