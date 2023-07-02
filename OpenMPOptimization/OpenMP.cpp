#include <iostream>
#include<Windows.h>
#include <omp.h>
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

#define N 8
#define num_threads 12
long long A[N][N], B[N][N], multi[N][N];

void matrix_multiply(long long A[N][N], long long B[N][N], long long C[N][N], int n, int threads) {
    omp_set_num_threads(num_threads); // �����߳���
#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
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
    matrix_multiply(A, B, multi, N, num_threads);
    tc.Stop();
    cout << tc.dbTime;
    return 0;
}