#include "iobench.h"
#include <stdio.h>

int main() {

    FILE * fp;
    fp = fopen ("data3", "w+");
    
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    // writes the characer '6' to the file
    size_t size = 5120000;
    const char* buf = "66666666";
    double start = tstamp();

    size_t n = 0;
    while (n < size) {
        ssize_t r = fwrite(buf, 1, sizeof(buf), fp);
        if (r < 1) {
            perror("write");
            exit(1);
        }
        // with some frequency (defined in iobench.h), prints out
        // how long it takes to complete the write.
        n += r;
        if (n % PRINT_FREQUENCY == 0) {
            report(n, tstamp() - start);
        }
    }

    fclose(fp);
    report(n, tstamp() - start);
    fprintf(stderr, "\n");
}
