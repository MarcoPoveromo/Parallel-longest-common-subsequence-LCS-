//
// Created by marco on 15/07/22.
//

#include "LCS_v1.h"

#include <mpi.h>
#include <random>
#include <string>

// A utility function to find min of two integers
int min(int a, int b)
{ return (a < b)? a: b; }

// A utility function to find min of three integers
int min3(int a, int b, int c)
{ return min(min(a, b), c);}

// A utility function to find max of two integers
int max(int a, int b)
{ return (a > b)? a: b; }

LCS_v1::LCS_v1(const std::string &x, const std::string &y) : LCS(x, y) {}

Result LCS_v1::computeLCS() {
    double start = MPI_Wtime();

    MPI_Bcast(const_cast<char*>(X.data()), X.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<char*>(Y.data()), Y.size(), MPI_CHAR, 0, MPI_COMM_WORLD);

    int rows = X.size() + 1;
    int cols = Y.size() + 1;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int row, col;
    MPI_Status status;

    int dp[3][cols];

    for (int line=1; line<rows+cols; line++) {
        int curr_line = line % 3;
        int prev_line = (line-1) % 3;
        int prev_prev_line = (line-2) % 3;

        int start_col =  max(0, line-rows);
        int count = min3(line, (cols-start_col), rows);

        int start, end;
        if (count <= size) {
            start = rank;
            end = min(rank, count-1);
        } else {
            float block_len = (float)count / size;
            start = round(block_len*rank);
            end = round(block_len*(rank+1))-1;
        }

        for (int j=start; j<=end; j++) {
            row = min(rows, line)-j-1;
            col = start_col+j;

            if (row==0 || col==0) {
                dp[curr_line][col] = 0;
            }
            else if (X[row - 1] == Y[col - 1]) {
                int upper_left = dp[prev_prev_line][col-1];
                dp[curr_line][col] = upper_left + 1;
            }
            else {
                int left = dp[prev_line][col-1];
                int up = dp[prev_line][col];
                dp[curr_line][col] = max(left, up);
            }

            if (j == start && rank > 0) {
                MPI_Send(&dp[curr_line][col], 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD);
            }
            if (j == end && rank < size-1) {
                MPI_Send(&dp[curr_line][col], 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD);
            }
        }

        int prev_index = start - 1;
        if (prev_index >= 0 && prev_index < count) {
            col = start_col+prev_index;
            MPI_Recv(&dp[curr_line][col], 1, MPI_INT, rank-1, 1, MPI_COMM_WORLD, &status);
        }

        int next_index = end + 1;
        if (next_index >= 0 && next_index < count) {
            col = start_col+next_index;
            MPI_Recv(&dp[curr_line][col], 1, MPI_INT, rank+1, 1, MPI_COMM_WORLD, &status);
        }
    }

    double end = MPI_Wtime();

    Result result(std::to_string(dp[(rows+cols-1) % 3][cols-1]));
    result.setTimeElapsed((end-start));

    return result;
}