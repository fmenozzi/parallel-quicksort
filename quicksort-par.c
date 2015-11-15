#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>

struct pair {
    int left;
    int right;
};

double wctime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1E-6 * tv.tv_usec;
}

int* prefix_sum(int X[], int lo, int hi) {
    int n = hi-lo+1;

    int* Y = malloc(sizeof(*Y) * n/2);
    int* S = malloc(sizeof(*S) * n);

    int i;

    S[0] = X[0];

    if (n > 1) {
        for (i = 0; i < n/2; i++)
            Y[i] = X[2*i] + X[2*i+1];
        
        int* Z = prefix_sum(Y, lo, lo + (hi-lo)/2);

        for (i = 1; i < n; i++)
            if (i % 2 == 1)
                S[i] = Z[i/2];
            else
                S[i] = Z[(i-1)/2] + X[i];

        free(Z);
    }

    free(Y);

    return S;
}

struct pair partition(double A[], int lo, int hi) {
    double Ap = A[lo + (hi-lo)/2];

    int n = hi-lo+1;

    int ltn = 0;
    int eqn = 0;
    int gtn = 0;

    int i;

    // Determine the sizes of each partition
    for (i = lo; i <= hi; i++)
        if (A[i] < Ap)
            ltn += 1;
    for (i = lo; i <= hi; i++)
        if (A[i] > Ap)
            gtn += 1;
    eqn = n-ltn-gtn;

    // Allocate partitions
    double* lt = malloc(sizeof(*lt) * ltn);
    double* eq = malloc(sizeof(*eq) * eqn);
    double* gt = malloc(sizeof(*gt) * gtn);

    // Populate masks
    int* ltmask = malloc(sizeof(*ltmask) * n);
    int* eqmask = malloc(sizeof(*eqmask) * n);
    int* gtmask = malloc(sizeof(*gtmask) * n);
    for (i = 0; i < n; i++) {
        ltmask[i] = A[lo + i] < Ap;
        gtmask[i] = A[lo + i] > Ap;
        eqmask[i] = !ltmask[i] && !gtmask[i];
    }

    // Calculate prefix sums
    int* ltsum = prefix_sum(ltmask, 0, n-1);
    int* eqsum = prefix_sum(eqmask, 0, n-1);
    int* gtsum = prefix_sum(gtmask, 0, n-1);

    // Populate lt, eq, and gt
    for (i = 0; i < n; i++) {
        if (ltmask[i])
            lt[ltsum[i]-1] = A[lo + i];
        if (eqmask[i])
            eq[eqsum[i]-1] = A[lo + i];
        if (gtmask[i])
            gt[gtsum[i]-1] = A[lo + i];
    }

    // Copy partitions into A
    for (i = 0; i < ltn; i++)
        A[lo + i] = lt[i];
    for (i = 0; i < eqn; i++)
        A[lo + ltn + i] = eq[i];
    for (i = 0; i < gtn; i++)
        A[lo + ltn + eqn + i] = gt[i];

    free(lt);
    free(eq);
    free(gt);

    free(ltmask);
    free(eqmask);
    free(gtmask);

    free(ltsum);
    free(eqsum);
    free(gtsum);

    struct pair idxs = {lo + ltn - 1, lo + ltn + eqn};
    return idxs;
}

void quicksort(double A[], int lo, int hi) {
    if (lo >= hi)
        return;

    struct pair p = partition(A, lo, hi);

    quicksort(A, lo, p.left);
    quicksort(A, p.right, hi);
}

int main(int argc, char* argv[]) {
    int i, N;
    double start, end;

    srand(123);

    if (argc != 2) {
        printf("Usage: ./quicksort-par N\n");
        return -1;
    }
    N = atol(argv[1]);

    double* A = malloc(sizeof(*A) * N);
    for (i = 0; i < N; i++)
        A[i] = (double)rand() / (double)RAND_MAX;

    start = wctime();
    quicksort(A, 0, N-1);
    end = wctime();

    printf("%f\n", end-start);

    free(A);

    return 0;
}
