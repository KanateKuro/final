#include <mpi.h>
#include <iostream>
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

const int N = 4096;
int A[N][N], B[N][N], C[N][N];
int main(int argc, char** argv) {
    int i, j, k, sum;

    // 初始化矩阵A和B
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i + j;
        }
    }

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // 计算每个进程需要计算的行数
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;

    // 每个进程计算自己负责的部分
    TimerCounter tc;
    tc.Start();
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < N; j++) {
            sum = 0;
            for (k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    tc.Stop();
    // 将每个进程计算的结果发送给进程0
    if (rank != 0) {
        MPI_Send(C[start_row], rows_per_process * N, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else {
        cout << tc.dbTime;
        for (i = 1; i < size; i++) {
            MPI_Recv(C[i * rows_per_process], rows_per_process * N, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        //for (i = 0; i < N; i++) {
            //for (j = 0; j < N; j++) {
                //cout << C[i][j] << " ";
            //}
            //cout << endl;
        //}
    }
    MPI_Finalize();
    return 0;
}