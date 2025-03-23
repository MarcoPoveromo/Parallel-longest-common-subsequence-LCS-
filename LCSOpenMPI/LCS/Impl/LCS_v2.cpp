//
// Created by marco on 15/07/22.
//

#include "LCS_v2.h"

#include <mpi.h>
#include <random>
#include <string>

LCS_v2::LCS_v2(const std::string &x, const std::string &y) : LCS(x, y) {}


int mapper(char x){
    if (x == 'A') return 0;
    if (x == 'C') return 1;
    if (x == 'G') return 2;
    return 3;
}

Result LCS_v2::computeLCS() {
    double start = MPI_Wtime();

    // Print process ranks
    int procRank, procSize;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    MPI_Bcast(const_cast<char*>(X.data()), X.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(const_cast<char*>(Y.data()), Y.size(), MPI_CHAR, 0, MPI_COMM_WORLD);

    int l = 4;
    int C[] = {'A', 'C', 'G', 'T'};

    int *P2 = (int *) malloc(sizeof(int) * (Y.size() + 1) * l);

    int nPerProc = l / procSize;
    int remaining = l % procSize;
    int rowToCompute[procSize];
    int rowToComputeSize[procSize];

    for(int i = 0; i < procSize; i++) {
        rowToCompute[i] = nPerProc;
    }
    for(int i = 0; i < remaining; i++) {
        rowToCompute[i] += 1;
    }

    for(int i = 0; i < procSize; i++) {
        rowToComputeSize[i] = rowToCompute[i] * (Y.size() + 1);
    }

    int *P = (int *) malloc(sizeof(int) * rowToComputeSize[procRank]);

    int rowOffsets[procSize]; for (int i=0; i < procSize; i++){ rowOffsets[i] = 0; }
    int offsets[procSize];
    int offset = 0;
    for (int i=0; i < procSize; i++){
        if(i>0) {
            offset += rowToCompute[i-1];
            rowOffsets[i] += offset;
        } else rowOffsets[i] = 0;
        offsets[i] = rowOffsets[i] * (Y.size() + 1);
    }

    for (int i = 0; i < rowToCompute[procRank]; i++) {
        for (int j = 0; j <= Y.size(); j++) {
            P[i * (Y.size()+1) + j] = 0;
        }
    }

    for (int i = 0; i < rowToCompute[procRank]; i++) {
        for (int j = 0; j <= Y.size(); j++) {
            if (j == 0) P[i * (Y.size() + 1) + j] = 0;
            else if (Y[j - 1] == C[rowOffsets[procRank] + i]) P[i * (Y.size() + 1) + j] = j;
            else P[i * (Y.size() + 1) + j] = P[i * (Y.size() + 1) + j - 1];
        }
    }

    MPI_Allgatherv(P, rowToComputeSize[procRank], MPI_INT,
                   P2, rowToComputeSize, offsets,
                   MPI_INT, MPI_COMM_WORLD);

    /*if(procRank == 0) {
        for (int i = 0; i < l; i++) {
            for (int j = 0; j <= Y.size(); j++) {
                std::cout << P2[i * (Y.size()+1) + j] << "  ";
            }
            std::cout << std::endl;
        }
    }*/

    free(P);

    int elementPerProcess[procSize];
    for (int i=0; i< procSize; i++) {
        elementPerProcess[i] = (Y.size() + 1) / procSize;
    }
    int elementRemaining = (Y.size()+1) % procSize;
    for(int i = 0; i < elementRemaining; i++) {
        elementPerProcess[i] += 1;
    }


    int *precRow = (int *) malloc((sizeof(int) * (Y.size()+1)));
    for (int i = 0; i <= Y.size(); i++) {
        precRow[i] = 0;
    }
    int *currentRow = (int *) malloc((sizeof(int) * elementPerProcess[procRank]));

    offsets[0] = 0;
    offset = 0;
    for (int i=1; i < procSize; i++){
        offset += elementPerProcess[i-1];
        offsets[i] = offset;
    }

    int jj, x_i, upper, P2Mapper;
    for (int i = 1; i <= X.size(); i++) {
        x_i = X[i-1];

        /*if(procRank == 1) {
            std::cout << "Rank0 prec i = " << i << "\n";
            for (int j=0; j <= Y.size(); j++) {
                std::cout << precRow[i] << " ";
            }
            std::cout << "\n";
        }*/

        for (int j = 0; j < elementPerProcess[procRank]; j++) {
            jj = j + offsets[procRank];
            upper = precRow[jj];
            P2Mapper = P2[mapper(x_i) * (Y.size()+1) + jj];

            bool t = std::signbit(0 - P2Mapper);
            bool s = std::signbit(0 - (upper - t * precRow[P2Mapper - 1]));
            //if(procRank == 0) std::cout << "t: "<< t << " s: " << s << std::endl;
            currentRow[j] = precRow[jj] + t * !s;
            //if(procRank == 0) std::cout << "currentRow[" << j << "] = " << currentRow[j] << "\n";
        }

        /*if(procRank == 1) {
            std::cout << "Rank["<< procRank << "] current i = " << i << "\n";
            for (int j=0; j < elementPerProcess[procRank]; j++) {
                currentRow[j] = 33;
                std::cout << currentRow[j] << " ";
            }
            std::cout << "\n";
            for (int j=0; j < procSize; j++) {
                std::cout << offsets[j] << " ";
            }
            std::cout << "\n";
        }*/

        MPI_Allgatherv(currentRow, elementPerProcess[procRank], MPI_INT,
                       precRow, elementPerProcess, offsets,
                       MPI_INT, MPI_COMM_WORLD);
    }

    /*if(procRank == 1) {
        std::cout << "\n\n";
        for (int i = 0; i <= 1; i++) {
            for (int j = 0; j < elementPerProcess; j++) {
                std::cout << M[i * elementPerProcess + j] << "  ";
            }
            std::cout << std::endl;
        }
    }*/
    double end = MPI_Wtime();

    Result result(std::to_string(currentRow[elementPerProcess[procRank]-1]));
    result.setTimeElapsed((end-start));

    return result;
}

