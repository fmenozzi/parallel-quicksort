#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/time.h>

double wctime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1E-6 * tv.tv_usec;
}

void swap(double* a, double* b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(double A[], int64_t lo, int64_t hi) {
    int64_t l = lo;
    int64_t r = hi;

    int64_t p = A[lo + (hi-lo)/2];

    while (1) {
        while (A[l] < p)
            l++;
        while (A[r] > p)
            r--;

        if (l < r)
            swap(&A[l], &A[r]);
        else
            return l;
    }

    return l;
}

void quicksort(double A[], int64_t lo, int64_t hi) {
    if (lo >= hi)
        return;

    int64_t p = partition(A, lo, hi);

    quicksort(A, lo, p);
    quicksort(A, p+1, hi);
}

/*
 * TODO: Runs fine up to and including N = 15357, but it hangs
 *       when N is any bigger
 *
 *       Adding srand() call increases it to 48559, but problem
 *       remains
 */

int main(int argc, char* argv[]) {
    int64_t i, N;
    double start, end;

    srand(123);

    if (argc != 2) {
        printf("Usage: ./quicksort-seq N\n");
        return -1;
    }
    N = atol(argv[1]);

    double* A = malloc(sizeof(*A) * N);
    if (!A) {
        printf("Unable to allocate memory for %ld doubles\n", N);
        return -1;
    }
    for (i = 0; i < N; i++)
        A[i] = (double)rand();

    start = wctime();
    quicksort(A, 0, N-1);
    end = wctime();

    printf("%f\n", end-start);

    return 0;
}
