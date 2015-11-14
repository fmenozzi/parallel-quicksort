#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

struct indices {
    int left;
    int right;
};

struct indices partition(int A[], int lo, int hi) {
    int* lt = malloc(sizeof(*lt) * (hi-lo+1));
    int* eq = malloc(sizeof(*eq) * (hi-lo+1));
    int* gt = malloc(sizeof(*gt) * (hi-lo+1));

    int Ap = A[lo + (hi-lo)/2];

    int ltn = 0;
    int eqn = 0;
    int gtn = 0;

    int i;

    // Populate lt, eq, and gt
    for (i = lo; i <= hi; i++) {
        if (A[i] < Ap) {
            lt[ltn++] = A[i];
        } else if (A[i] > Ap) {
            gt[gtn++] = A[i];
        } else {
            eq[eqn++] = A[i];
        }
    }

    // Partition A
    #pragma omp parallel for private(i)
    for (i = 0; i < ltn; i++)
        A[lo + i] = lt[i];
    #pragma omp parallel for private(i)
    for (i = 0; i < eqn; i++)
        A[lo + ltn + i] = eq[i];
    #pragma omp parallel for private(i)
    for (i = 0; i < gtn; i++)
        A[lo + ltn + eqn + i] = gt[i];

    free(lt);
    free(eq);
    free(gt);

    struct indices idxs = {lo + ltn - 1, lo + ltn + eqn};
    return idxs;
}

void quicksort(int A[], int lo, int hi) {
    if (lo >= hi)
        return;

    struct indices i = partition(A, lo, hi);

    quicksort(A, lo, i.left);
    quicksort(A, i.right, hi);
}

#define N 16

int main() {
    int A[N] = {1,3,4,3,2,2,1,4,1,3,4,3,2,2,1,4};

    quicksort(A, 0, N-1);

    int i;
    for (i = 0; i < N; i++)
        printf("%d ", A[i]);
    printf("\n");

    return 0;
}
