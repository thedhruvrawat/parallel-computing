#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // omp_set_num_threads(8);
    printf("Hello World\n");
    printf("No. of parallel process possible: %d\n", omp_get_num_procs());
#pragma omp parallel 
    {
        // printf("I am a parallel region.\n");
        printf("Hi I'm parallel process no. : %d\n", omp_get_thread_num());
    }
    return 0;
}