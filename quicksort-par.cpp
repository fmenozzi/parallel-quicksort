#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <sys/time.h>

#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

const int N;
const int P = 20;

struct pair {
    int left;
    int right;
};

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

int* prefix_sum(int X[], int lo, int hi) {
    int n = hi-lo+1;

    int* Y = new int[n/2];
    int* S = new int[n];

    S[0] = X[0];

    if (n > 1) {
        cilk_for (int i = 0; i < n/2; i++)
            Y[i] = X[2*i] + X[2*i+1];
        
        int* Z = prefix_sum(Y, lo, lo + (hi-lo)/2);

        cilk_for (int i = 1; i < n; i++)
            if (i % 2 == 1)
                S[i] = Z[i/2];
            else
                S[i] = Z[(i-1)/2] + X[i];

        delete[] Z;
    }

    delete[] Y;

    return S;
}

struct pair partition(double A[], int lo, int hi) {
    double Ap = A[lo + (hi-lo)/2];

    int n = hi-lo+1;

    // Determine the sizes of each partition
    cilk::reducer_opadd<double> ltn_reducer;
    cilk::reducer_opadd<double> gtn_reducer;
    cilk_for (int i = lo; i <= hi; i++) {
        if (A[i] < Ap)
            ltn_reducer += 1;
        else if (A[i] > Ap)
            gtn_reducer += 1;
    }
    int ltn = ltn_reducer.get_value();
    int gtn = gtn_reducer.get_value();
    int eqn = n-ltn-gtn;

    // Allocate partitions
    // TODO: See if you can't just allocate a single
    //       array of size n instead
    double* lt = new double[ltn];
    double* eq = new double[eqn];
    double* gt = new double[gtn];

    // Populate masks
    int* ltmask = new int[n];
    int* eqmask = new int[n];
    int* gtmask = new int[n];
    cilk_for (int i = 0; i < n; i++) {
        ltmask[i] = A[lo + i] < Ap;
        gtmask[i] = A[lo + i] > Ap;
        eqmask[i] = !ltmask[i] && !gtmask[i];
    }

    // Calculate prefix sums
    int* ltsum = prefix_sum(ltmask, 0, n-1);
    int* eqsum = prefix_sum(eqmask, 0, n-1);
    int* gtsum = prefix_sum(gtmask, 0, n-1);

    // Populate lt, eq, and gt
    cilk_for (int i = 0; i < n; i++) {
        if (ltmask[i])
            lt[ltsum[i]-1] = A[lo + i];
        if (eqmask[i])
            eq[eqsum[i]-1] = A[lo + i];
        if (gtmask[i])
            gt[gtsum[i]-1] = A[lo + i];
    }

    // Copy partitions into A
    cilk_for (int i = 0; i < ltn; i++)
        A[lo + i] = lt[i];
    cilk_for (int i = 0; i < eqn; i++)
        A[lo + ltn + i] = eq[i];
    cilk_for (int i = 0; i < gtn; i++)
        A[lo + ltn + eqn + i] = gt[i];

    delete[] lt;
    delete[] eq;
    delete[] gt;

    delete[] ltmask;
    delete[] eqmask;
    delete[] gtmask;

    delete[] ltsum;
    delete[] eqsum;
    delete[] gtsum;

    struct pair idxs;
    idxs.left  = lo + ltn - 1;
    idxs.right = lo + ltn + eqn;
    return idxs;
}

void quicksort(double A[], int lo, int hi) {
    if (lo >= hi)
        return;

    int n = hi-lo+1;
    if (n < N/P) {
        qsort(A, n, sizeof(*A), lt);
        return;
    }

    struct pair p = partition(A, lo, hi);

    cilk_spawn quicksort(A, lo, p.left);
    cilk_spawn quicksort(A, p.right, hi);

    cilk_sync;
}

int main(int argc, char* argv[]) {
    int i;
    double start, end;

    srand(123);

    if (argc != 2) {
        printf("Usage: ./quicksort-par N\n");
        return -1;
    }
    N = atol(argv[1]);

    double* A = new double[N];
    for (i = 0; i < N; i++)
        A[i] = (double)rand() / (double)RAND_MAX;

    start = wctime();
    cilk_spawn quicksort(A, 0, N-1);
    cilk_sync;
    end = wctime();

    printf("%f\n", end-start);

    delete[] A;

    return 0;
}
