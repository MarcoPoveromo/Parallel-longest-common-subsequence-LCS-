//
// Created by marco on 18/07/22.
//

#include "LCS_v2.cuh"
#include "../../cuda_launch_config.cu"

// --------------------------------- BEGIN CUDA ------------------------------------------------
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); }
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort=true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr,"GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

__device__
int mapper(char x){
    if (x == 'A') return 0;
    if (x == 'C') return 1;
    if (x == 'G') return 2;
    return 3;
}

__global__
void computeLcs_v2(int *precRow, int *currentRow, int *P, int ySize, char *X, int i, char x_i) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    if(j == 0) {
        currentRow[j] = 0;
        return;
    }

    bool t, s;
    int upper, PMapper;

    while(j <= ySize) {
        upper = precRow[j];
        PMapper = P[mapper(x_i) * (ySize+1) + j];

        t = (0 - PMapper) < 0;
        s = (0 - (upper - t * precRow[PMapper - 1])) < 0;
        currentRow[j] = precRow[j] + t * !s;

        j += blockDim.x * gridDim.x;
    }
}

// ---------------------------------- END CUDA -------------------------------------------------

LCS_v2::LCS_v2(const std::string &x, const std::string &y, const dim3 &nBlocks, const dim3 &nThreadsPerBlock)
        : LCSStrategy(x, y), nBlocks(nBlocks), nThreadsPerBlock(nThreadsPerBlock) {}

LCS_v2::LCS_v2(const std::string &x, const std::string &y) : LCSStrategy(x, y) {}

const dim3 &LCS_v2::getNBlocks() const {
    return nBlocks;
}

void LCS_v2::setNBlocks(const dim3 &nBlocks) {
    this->nBlocks = nBlocks;
}

const dim3 &LCS_v2::getNThreadsPerBlock() const {
    return nThreadsPerBlock;
}

void LCS_v2::setNThreadsPerBlock(const dim3 &nThreadsPerBlock) {
    this->nThreadsPerBlock = nThreadsPerBlock;
}

Result LCS_v2::computeLCS() {
    // Start timer event
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    size_t size = sizeof(int) * (Y.size() + 1);
    int *d_currentRow, *d_precRow;
    cudaMalloc(&d_precRow, size);
    cudaMalloc(&d_currentRow, size);
    cudaMemset(d_currentRow, 0, size);
    // Copy X and Y on GPU
    char *d_X;
    cudaMalloc(&d_X, X.size() + 1);
    cudaMemcpy(d_X, X.c_str(), sizeof(char) * (X.size() + 1), cudaMemcpyHostToDevice);
    int l = 4;
    int C[] = {'A', 'C', 'G', 'T'};

    // Initialize P lookups table
    size_t size_P = sizeof(int) * l * (Y.size() + 1);
    int *P = (int *) malloc(size_P);
    int *d_P;
    cudaMalloc(&d_P, size_P);

    for (int i = 0; i < l; i++) {
        for (int j = 0; j <= Y.size(); j++) {
            if(j == 0) P[i * (Y.size()+1) + j] = 0;
            else if (Y[j-1] == C[i]) P[i * (Y.size()+1) + j] = j;
            else P[i * (Y.size()+1) + j] = P[i * (Y.size()+1) + j - 1];
        }
    }

    cudaMemcpy(d_P, P, size_P, cudaMemcpyHostToDevice);

    // Maximize occupancy https://github.com/jaredhoberock/cuda_launch_config
    size_t num_threads = block_size_with_maximum_potential_occupancy(computeLcs_v2);
    size_t num_blocks = (Y.size()+1) / num_threads;
    if((Y.size()+1) % num_threads) ++num_blocks;

    for(int i = 1; i <= X.size(); i++) {
        char x_i = X[i-1];
        if(i%2==0) computeLcs_v2<<<num_blocks, num_threads>>>(d_precRow, d_currentRow, d_P, Y.size(), d_X, i, x_i);
        else computeLcs_v2<<<num_blocks, num_threads>>>(d_currentRow, d_precRow, d_P, Y.size(), d_X, i, x_i);
    }

    int *lcs = (int *) malloc(sizeof(int));
    if((X.size()) %2==0) cudaMemcpy(lcs, &(d_currentRow[Y.size()]), sizeof(int), cudaMemcpyDeviceToHost);
    else cudaMemcpy(lcs, &(d_precRow[Y.size()]), sizeof(int), cudaMemcpyDeviceToHost);


    // Stop timer event
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    // Build the result object
    Result result(std::to_string(*lcs));
    result.setTimeElapsed(milliseconds);

    cudaFree(d_precRow); cudaFree(d_currentRow); cudaFree(d_X);

    return result;
}
