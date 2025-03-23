//
// Created by marco on 15/07/22.
//

#include "LCS_v1.h"
#include <string>
#include <chrono>

LCS_v1::LCS_v1(const std::string &x, const std::string &y) : LCS(x, y) {}

Result LCS_v1::computeLCS() {
    auto t1 = std::chrono::high_resolution_clock::now();

    // Allocate 3 rows of the matrix
    size_t size = sizeof(int) * (Y.size() + 1);
    int *precPrecDiagonal = (int *) malloc(size);
    int *precDiagonal = (int *) malloc(size);
    int *currentDiagonal = (int *) malloc(size);

    int m = Y.size();
    int n = X.size();

    int diagonalSize;
    int a, b;

    for (int i=0, j=0; i <= m && j <= n; j++) {
        diagonalSize = std::min(j, m - i);

        #pragma omp parallel for private(a, b)
        for (int d = 0; d <= diagonalSize; d++) {
            a = i + d;
            b = j - d;

            if (a == 0 || b == 0) currentDiagonal[b] = 0;
            else if (X[a - 1] == Y[b - 1]) currentDiagonal[b] = precPrecDiagonal[b - 1] + 1;
            else currentDiagonal[b] = std::max(precDiagonal[b], precDiagonal[b - 1]);
        }

        if(j == n){ j--; i++; }

        // Swap pointer of the 3 rows
        int *temp = precDiagonal;
        precDiagonal = currentDiagonal;
        int *temp2 = precPrecDiagonal;
        precPrecDiagonal = temp;
        currentDiagonal = temp2;
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

    // Build lcs
    Result result(std::to_string(precDiagonal[n]));
    result.setTimeElapsed(timeElapsed);

    // Free memory
    free(precPrecDiagonal);
    free(precDiagonal);
    free(currentDiagonal);

    return result;
}