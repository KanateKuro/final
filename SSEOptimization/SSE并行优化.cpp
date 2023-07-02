#include<iostream>
#include<Windows.h>
#include<emmintrin.h>
#include<immintrin.h>
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

#define N 4096
float A[N][N], B[N][N], multi[N][N];
void  SSEOptimization(float* A, float* B, float* C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            __m128 c = _mm_setzero_ps();
            for (int k = 0; k < n; k += 4) {
                __m128 a = _mm_loadu_ps(&A[i * n + k]);
                __m128 b = _mm_loadu_ps(&B[j * n + k]);
                c = _mm_add_ps(c, _mm_mul_ps(a, b));
            }
            float temp[4];
            _mm_storeu_ps(temp, c);
            C[i * n + j] = temp[0] + temp[1] + temp[2] + temp[3];
        }
    }
}
int main() {
    TimerCounter tc;
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i + j;
            multi[i][j] = 0;
        }
    }
    tc.Start();
    SSEOptimization(&A[0][0], &B[0][0], &multi[0][0], N);
    tc.Stop();
    cout << tc.dbTime;
}