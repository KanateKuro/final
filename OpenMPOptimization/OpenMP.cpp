#include <iostream>
#include<Windows.h>
#include <omp.h>
using namespace std;

class TimerCounter {
public:
    TimerCounter(void);//构造函数
    ~TimerCounter(void);//析构函数
private:
    LARGE_INTEGER startCount;//记录开始时间

    LARGE_INTEGER endCount;//记录结束时间

    LARGE_INTEGER freq;//本机CPU时钟频率
public:
    double dbTime;//程序运行的时间保存在这里

public:
    void Start();//被测程序开始点处开始计时
    void Stop();//被测程序结束点处结束计时
};
TimerCounter::TimerCounter(void) {
    QueryPerformanceFrequency(&freq);//获取主机CPU时钟频率
}
TimerCounter::~TimerCounter(void) {
}
void TimerCounter::Start() {
    QueryPerformanceCounter(&startCount);//开始计时
}
void TimerCounter::Stop() {
    QueryPerformanceCounter(&endCount);//停止计时
    dbTime = ((double)endCount.QuadPart - (double)startCount.QuadPart) / (double)freq.QuadPart;//获取时间差
}

#define N 8
#define num_threads 12
long long A[N][N], B[N][N], multi[N][N];

void matrix_multiply(long long A[N][N], long long B[N][N], long long C[N][N], int n, int threads) {
    omp_set_num_threads(num_threads); // 设置线程数
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