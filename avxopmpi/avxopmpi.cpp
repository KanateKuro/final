#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <immintrin.h>
using namespace std;

const int N = 4096;
const int num_threads = 12;
float A[N][N], B[N][N], C[N][N];

void print_matrix(float(*matrix)[N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void init_matrices() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i + j;
            C[i][j] = 0;
        }
    }
}

void matrix_mul(float* A, float* B, float* C, int n) {
    omp_set_num_threads(num_threads);
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            __m256 c = _mm256_setzero_ps();
            for (int k = 0; k < N; k += 8) {
                __m256 a = _mm256_loadu_ps(A + i * N + k);
                __m256 b = _mm256_loadu_ps(B + k * N + j);
                c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
            }
            _mm256_storeu_ps(C + i * N + j, c);
        }
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(rank + time(nullptr));
    init_matrices();

    //if (rank == 0) {
        //print_matrix(A);
        //print_matrix(B);
    //}
    matrix_mul(&A[0][0], &B[0][0], &C[0][0], N);

    //if (rank == 0) {
        //print_matrix(C);
    //}

    MPI_Finalize();
    return 0;
}
