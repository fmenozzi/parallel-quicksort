#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

double wctime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1E-6 * tv.tv_usec;
}

int lt(const void* a, const void* b) {
    double aa = *(double*)a;
    double bb = *(double*)b;

    if (aa < bb)
        return -1;
    if (aa > bb)
        return 1;
    return 0;
}

int main(int argc, char* argv[]) {
    int i, N;
    double start, end;

    srand(123);

    if (argc != 2) {
        printf("Usage: ./quicksort-seq N\n");
        return -1;
    }
    N = atol(argv[1]);

    double* A = malloc(sizeof(*A) * N);
    for (i = 0; i < N; i++)
        A[i] = (double)rand() / (double)RAND_MAX;

    start = wctime();
    qsort(A, N, sizeof(*A), lt);
    end = wctime();

    printf("%f\n", end-start);

    free(A);

    return 0;
}
