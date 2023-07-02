#include <mpi.h>
#include <iostream>
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

const int N = 4096;
int A[N][N], B[N][N], C[N][N];
int main(int argc, char** argv) {
    int i, j, k, sum;

    // ��ʼ������A��B
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
    // ����ÿ��������Ҫ���������
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;

    // ÿ�����̼����Լ�����Ĳ���
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
    // ��ÿ�����̼���Ľ�����͸�����0
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