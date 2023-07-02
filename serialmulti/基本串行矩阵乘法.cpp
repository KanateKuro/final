#include<iostream>
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