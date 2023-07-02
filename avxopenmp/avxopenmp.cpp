#include <iostream>
#include <immintrin.h>
#include <omp.h>

const int N = 4096;
const int num_threads = 12;
void matmul_avx_openmp(float* A, float* B, float* C, int N) {
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

int main(){
    float* A = new float[N * N];
    float* B = new float[N * N];
    float* C = new float[N * N];

    // 初始化 A 和 B 矩阵
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = i + j;
            B[i * N + j] = i + j;
        }
    }
    matmul_avx_openmp(A, B, C, N);
    return 0;
}

