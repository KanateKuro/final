#include<iostream>
#include<Windows.h>
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

#define N 4096
float a[N][N], b[N][N], multi[N][N];
void serialmulti(float a[N][N], float b[N][N], float multi[N][N]) {
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            for (long long k = 0; k < N; k++) {
                multi[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}
int main() {
    TimerCounter tc;
    for (long long i = 0; i < N; i++) {
        for (long long j = 0; j < N; j++) {
            a[i][j] = i + j;
            b[i][j] = i + j;
            multi[i][j] = 0;
        }
    }
    tc.Start();
    serialmulti(a, b, multi);
    tc.Stop();
    cout << tc.dbTime;
}