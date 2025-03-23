//
// Created by marco on 18/07/22.
//

#include "LCS_v1.cuh"

#include "../../cuda_launch_config.cu"

// --------------------------------- BEGIN CUDA ------------------------------------------------
/* Old kernel that use the entire matrix
__global__
void lcs(int *M, int i, int j, char *X, char *Y, int ySize, int diagonalSize) {
    int d = blockIdx.x * blockDim.x + threadIdx.x;
    int a, b;
    while (d <= diagonalSize) {

        a = i + d;
        b = j - d;

        if (a == 0 || b == 0) M[a * (ySize + 1) + b] = 0;
        else if (X[a - 1] == Y[b - 1]) M[a * (ySize + 1) + b] = M[(a - 1) * (ySize + 1) + b - 1] + 1;
        else M[a * (ySize + 1) + b] = max(M[(a - 1) * (ySize + 1) + b], M[a * (ySize + 1) + b - 1]);

        d += blockDim.x * gridDim.x;
    }
}*/

__global__
void lcs(int *precPrecDiagonal, int *precDiagonal, int *currentDiagonal, int i, int j, char *X, char *Y, int diagonalSize) {
    int d = blockIdx.x * blockDim.x + threadIdx.x;

    int a, b;

    while (d <= diagonalSize) {
        a = i + d;
        b = j - d;

        if (a == 0 || b == 0) currentDiagonal[b] = 0;
        else if (X[a - 1] == Y[b - 1]) currentDiagonal[b] = precPrecDiagonal[b - 1] + 1;
        else currentDiagonal[b] = max(precDiagonal[b], precDiagonal[b - 1]);

        d += blockDim.x * gridDim.x;
    }
}
// ---------------------------------- END CUDA -------------------------------------------------

LCS_v1::LCS_v1(const std::string &x, const std::string &y, const dim3 &nBlocks, const dim3 &nThreadsPerBlock)
        : LCSStrategy(x, y), nBlocks(nBlocks), nThreadsPerBlock(nThreadsPerBlock) {}

LCS_v1::LCS_v1(const std::string &x, const std::string &y) : LCSStrategy(x, y) {}

const dim3 &LCS_v1::getNBlocks() const {
    return nBlocks;
}

void LCS_v1::setNBlocks(const dim3 &nBlocks) {
    this->nBlocks = nBlocks;
}

const dim3 &LCS_v1::getNThreadsPerBlock() const {
    return nThreadsPerBlock;
}

void LCS_v1::setNThreadsPerBlock(const dim3 &nThreadsPerBlock) {
    this->nThreadsPerBlock = nThreadsPerBlock;
}

/* Old compute LCS
Result LCS_v1::computeLCS() {
    // Allocate memory for the 2D matrix in GPU
    int *d_M;
    size_t size = sizeof(int) * (X.size() + 1) * (Y.size() + 1);
    cudaMalloc(&d_M, size);

    // Copy X and Y on GPU
    char *d_X, *d_Y;
    cudaMalloc(&d_X, X.size() + 1);
    cudaMalloc(&d_Y, Y.size() + 1);
    cudaMemcpy(d_X, X.c_str(), sizeof(char) * (X.size() + 1), cudaMemcpyHostToDevice);
    cudaMemcpy(d_Y, Y.c_str(), sizeof(char) * (Y.size() + 1), cudaMemcpyHostToDevice);

    int m = Y.size();
    int n = X.size();
    int diagonalSize;

    for (int i=0, j=0; i <= m && j <= n; j++) {
        diagonalSize = std::min(j, m - i);
        lcs<<<20, 512>>>(d_M, i, j, d_X, d_Y, Y.size(), diagonalSize);
        if(j == n){ j--; i++; }
    }
    cudaDeviceSynchronize();


    int *M = (int *) malloc(size);
    cudaMemcpy(M, d_M, size, cudaMemcpyDeviceToHost);

    // Put the result in string for the moment
    std::string s = std::to_string(M[(X.size()+1) * (Y.size()+1) - 1]);

    // Free memory
    free(M); cudaFree(d_M); cudaFree(d_X); cudaFree(d_Y);

    return Result(s);
}*/

Result LCS_v1::computeLCS() {
    // Start timer event
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    // Allocate memory for the 2D matrix in GPU
    int *d_precPrecDiagonal, *d_precDiagonal, *d_currentDiagonal;
    size_t size = sizeof(int) * (Y.size() + 1);
    cudaMalloc(&d_precPrecDiagonal, size);
    cudaMalloc(&d_precDiagonal, size);
    cudaMalloc(&d_currentDiagonal, size);

    // Copy X and Y on GPU
    char *d_X, *d_Y;
    cudaMalloc(&d_X, X.size() + 1);
    cudaMalloc(&d_Y, Y.size() + 1);
    cudaMemcpy(d_X, X.c_str(), sizeof(char) * (X.size() + 1), cudaMemcpyHostToDevice);
    cudaMemcpy(d_Y, Y.c_str(), sizeof(char) * (Y.size() + 1), cudaMemcpyHostToDevice);

    int m = Y.size();
    int n = X.size();
    int diagonalSize;

    size_t num_threads = block_size_with_maximum_potential_occupancy(lcs);

    for (int i=0, j=0; i <= m && j <= n; j++) {
        diagonalSize = std::min(j, m - i);

        lcs<<<ceil((diagonalSize+1) / (double)num_threads), num_threads>>>(d_precPrecDiagonal, d_precDiagonal, d_currentDiagonal, i, j, d_X, d_Y, diagonalSize);
        if(j == n){ j--; i++; }

        int *temp = d_precDiagonal;
        d_precDiagonal = d_currentDiagonal;
        int *temp2 = d_precPrecDiagonal;
        d_precPrecDiagonal = temp;
        d_currentDiagonal = temp2;
    }

    // Retrive lcs from GPU
    int *lcs = (int *) malloc(sizeof(int));
    cudaMemcpy(lcs, &(d_precDiagonal[n]), sizeof(int), cudaMemcpyDeviceToHost);

    // Stop timer event
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    // Build the result object
    Result result(std::to_string(*lcs));
    result.setTimeElapsed(milliseconds);

    // Free memory
    cudaFree(d_precDiagonal);
    cudaFree(d_currentDiagonal);
    cudaFree(d_Y);
    cudaFree(d_X);

    return result;
}