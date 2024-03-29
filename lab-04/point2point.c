#include<mpi.h>
#include<stdio.h>

double func(double x) {
  return (double)x * x;
}

double Trap(double a, double b, int n, double h) {
  double area = (func(a) + func(b)) / 2.0;
  for (int i = 1; i <= n - 1; ++i) {
    double x = a + i * h;
    area += func(x);
  }
  area *= h;
  return area;
}

int main() {
  int my_rank, comm_sz, n = 1024, local_n;
  double a = 0.0, b = 3.0, h, local_a, local_b;
  double start = 0.0, end = 0.0;
  double local_int, total_int;
  int source;
  MPI_Init(NULL, NULL);
  start = MPI_Wtime();
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  h = (b - a) / n; /* h is the same for all processes */
  local_n = n / comm_sz; /* So is the number of trapezoids */
  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;
  local_int = Trap(local_a, local_b, local_n, h);
  if (my_rank != 0) {

    MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

  }
  else {

    total_int = local_int;
    for (source = 1; source < comm_sz; source++) {
      MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD,

        MPI_STATUS_IGNORE);

      total_int += local_int;

    }
  }
  if (my_rank == 0) {

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n",
      a, b, total_int);
    end = MPI_Wtime();
    printf("Time taken - %f\n", end-start);
  }
  MPI_Finalize();
  return 0;
}