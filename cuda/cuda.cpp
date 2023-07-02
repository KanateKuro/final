#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>

#define BLOCK_SIZE 16

__global__ void matrixMulCUDA(float* c, float* a, float* b, int w)
{
    int bx = blockIdx.x;
    int by = blockIdx.y;

    int tx = threadIdx.x;
    int ty = threadIdx.y;

    float sum = 0.0f;

    int ia = w * BLOCK_SIZE * by + w * ty;
    int ib = BLOCK_SIZE * bx + tx;

    for (int k = 0; k < w; k++)
    {
        float aElement = a[ia + k];
        float bElement = b[k * w + ib];
        sum += aElement * bElement;
    }

    int ic = w * BLOCK_SIZE * by + BLOCK_SIZE * bx;
    c[ic + w * ty + tx] = sum;
}

void constantInit(float* data, int size, float val)
{
    for (int i = 0; i < size; ++i)
    {
        data[i] = val;
    }
}

int main(int argc, char** argv)
{
    int devID = 0;
    cudaSetDevice(devID);

    int matrixWidth = 320;
    int matrixHeight = 320;

    dim3 dimsA(matrixWidth, matrixHeight, 1);
    dim3 dimsB(matrixWidth, matrixHeight, 1);

    int matrixSize = dimsA.x * dimsA.y;
    int matrixMemSize = sizeof(float) * matrixSize;

    float* h_A = (float*)malloc(matrixMemSize);
    float* h_B = (float*)malloc(matrixMemSize);
    float* h_C = (float*)malloc(matrixMemSize);

    constantInit(h_A, matrixSize, 1.0f);
    constantInit(h_B, matrixSize, 0.01f);

    float* d_A, * d_B, * d_C;

    cudaMalloc((void**)&d_A, matrixMemSize);
    cudaMalloc((void**)&d_B, matrixMemSize);
    cudaMalloc((void**)&d_C, matrixMemSize);

    cudaMemcpy(d_A, h_A, matrixMemSize, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, matrixMemSize, cudaMemcpyHostToDevice);

    dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
    dim3 grid(matrixWidth / threads.x, matrixHeight / threads.y);

    matrixMulCUDA << <grid, threads >> > (d_C, d_A, d_B, dimsA.x);

    cudaMemcpy(h_C, d_C, matrixMemSize, cudaMemcpyDeviceToHost);

    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    free(h_A);
    free(h_B);
    free(h_C);

    cudaDeviceReset();

    return 0;
}