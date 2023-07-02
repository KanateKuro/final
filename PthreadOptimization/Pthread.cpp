#include <iostream>
#include <pthread.h>

using namespace std;

const int N = 1000; // �����С
const int THREAD_NUM = 4; // �߳���

int A[N][N], B[N][N], C[N][N]; // ����

struct ThreadArg {
    int start, end;
};

void* multiply(void* arg) {
    ThreadArg* threadArg = (ThreadArg*)arg;
    int start = threadArg->start, end = threadArg->end;
    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    // ��ʼ������
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }

    // �����߳�
    pthread_t threads[THREAD_NUM];
    ThreadArg threadArgs[THREAD_NUM];
    int step = N / THREAD_NUM;
    for (int i = 0; i < THREAD_NUM; i++) {
        threadArgs[i].start = i * step;
        threadArgs[i].end = (i + 1) * step;
        if (i == THREAD_NUM - 1) {
            threadArgs[i].end = N;
        }
        pthread_create(&threads[i], NULL, multiply, (void*)&threadArgs[i]);
    }

    // �ȴ��߳̽���
    for (int i = 0; i < THREAD_NUM; i++) {
        pthread_join(threads[i], NULL);
    }

    // ������
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
