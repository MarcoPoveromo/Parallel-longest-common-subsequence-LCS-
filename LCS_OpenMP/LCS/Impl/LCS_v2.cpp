

#include "LCS_v2.h"

#include <random>
#include <string>
#include <chrono>
#include <iostream>

using namespace std;

LCS_v2::LCS_v2(const std::string &x, const std::string &y) : LCS(x, y) {}

int mapper(char x){
    if (x == 'A') return 0;
    if (x == 'C') return 1;
    if (x == 'G') return 2;
    return 3;
}

Result LCS_v2::computeLCS() {
    auto t1 = std::chrono::high_resolution_clock::now();

    size_t size = sizeof(int) * (Y.size() + 1);
    int *currentRow = (int *) malloc(size);
    int *precRow = (int *) malloc(size);

    int l = 4;
    int C[] = {'A', 'C', 'G', 'T'};

    // Initialize P lookups table
    size_t size_P = sizeof(int) * l * (Y.size() + 1);
    int *P = (int *) malloc(size_P);

    // Initialize the first row
    #pragma omp parallel for
    for (int j = 0; j <= Y.size(); j++) currentRow[j] = 0;

    #pragma omp parallel for
    for (int i = 0; i < l; i++) {
        for (int j = 0; j <= Y.size(); j++) {
            if(j == 0) P[i * (Y.size()+1) + j] = 0;
            else if (Y[j-1] == C[i]) P[i * (Y.size()+1) + j] = j;
            else P[i * (Y.size()+1) + j] = P[i * (Y.size()+1) + j - 1];
        }
    }

    int mapperX_i;
    bool t, s;

    for(int i = 1; i <= X.size(); i++) {
        mapperX_i = mapper(X[i-1]);

        #pragma omp parallel for private(t, s)
        for (int j = 0; j <= Y.size(); j++) {
            if(j == 0) {
                currentRow[j] = 0;
                continue;
            }

            int PMapper = P[mapperX_i * (Y.size()+1) + j];
            int upper = precRow[j];

            t = (0 - PMapper) < 0;
            s = (0 - (upper - t * precRow[PMapper - 1])) < 0;
            currentRow[j] = precRow[j] + t * !s;
        }

        // Swap currentRow and precRow pointers
        int *temp = precRow;
        precRow = currentRow;
        currentRow = temp;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    // Build lcs
    Result result(std::to_string(precRow[(Y.size())]));
    result.setTimeElapsed(timeElapsed);

    free(P);
    free(precRow);
    free(currentRow);

    return result;
}

