#include <iostream>
#include <immintrin.h>
#include <pthread.h>

using namespace std;


const int N = 4096 ;
const int NUM_THREADS = 12;

float *A, *B, *C;
int current_row = 0;
pthread_mutex_t mutex;

void* matrix_multiply(void* arg) {
    int thread_id = *(int*)arg;
    while (true) {
        pthread_mutex_lock(&mutex);
        int row = current_row;
        current_row++;
        pthread_mutex_unlock(&mutex);
        if (row >= N) {
            break;
        }
        for (int i = 0; i < N; i++) {
          for (int j = 0; j < N; j++) {
              __m256 c = _mm256_setzero_ps();
              for (int k = 0; k < N; k += 8) {
                  __m256 a = _mm256_loadu_ps(A + i*N + k);
                  __m256 b = _mm256_loadu_ps(B + k*N + j);
                  c = _mm256_add_ps(c, _mm256_mul_ps(a, b));
              }
              _mm256_storeu_ps(C + i*N + j, c);
          }
      }
    }
    pthread_exit(NULL);
}

int main() {
    A = new float[N * N];
    B = new float[N * N];
    C = new float[N * N];
    // ³õÊ¼»¯ A ºÍ B ¾ØÕó
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = i + j;
            B[i * N + j] = i + j;
        }
    }
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, matrix_multiply, (void*)&thread_ids[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
