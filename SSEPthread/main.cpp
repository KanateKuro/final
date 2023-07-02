#include <iostream>
#include <immintrin.h>
#include <pthread.h>
#include<Windows.h>

using namespace std;

class TimerCounter {
public:
    TimerCounter(void);//���캯��
    ~TimerCounter(void);//��������
private:
    LARGE_INTEGER startCount;//��¼��ʼʱ��

    LARGE_INTEGER endCount;//��¼����ʱ��

    LARGE_INTEGER freq;//����CPUʱ��Ƶ��
public:
    double dbTime;//�������е�ʱ�䱣��������

public:
    void Start();//�������ʼ�㴦��ʼ��ʱ
    void Stop();//�����������㴦������ʱ
};
TimerCounter::TimerCounter(void) {
    QueryPerformanceFrequency(&freq);//��ȡ����CPUʱ��Ƶ��
}
TimerCounter::~TimerCounter(void) {
}
void TimerCounter::Start() {
    QueryPerformanceCounter(&startCount);//��ʼ��ʱ
}
void TimerCounter::Stop() {
    QueryPerformanceCounter(&endCount);//ֹͣ��ʱ
    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//��ȡʱ���
}
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
        for (int j = 0; j < N; j++) {
            __m128 c = _mm_setzero_ps();
            for (int k = 0; k < N; k += 4) {
                __m128 a = _mm_load_ps(&A[row * N + k]);
                __m128 b = _mm_load_ps(&B[j * N + k]);
                c = _mm_add_ps(c, _mm_mul_ps(a, b));
            }
            C[row * N + j] = ((float*)&c)[0] + ((float*)&c)[1] + ((float*)&c)[2] + ((float*)&c)[3];
        }
    }
    pthread_exit(NULL);
}

int main() {
    TimerCounter tc;
    A = new float[N * N];
    B = new float[N * N];
    C = new float[N * N];
    // ��ʼ�� A �� B ����
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = i + j;
            B[i * N + j] = i + j;
        }
    }
    tc.Start();
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
    tc.Stop();
    cout<<tc.dbTime;
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
