#include <iostream>
#include <bits/stdc++.h>
#include "Result.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

/*
 * Global variables
 */
string X, Y;
string filePath;

/*
 * Read the sequences file
 */
void readFile() {
    fstream newfile;
    newfile.open(filePath, ios::in);
    if (newfile.is_open()){
        string tp;
        getline(newfile, tp); // Reject the first line
        getline(newfile, tp); // Get X
        X = string(tp);
        getline(newfile, tp); // Get Y
        Y = string(tp);
        newfile.close();
    } else {
        cout << "File not found\n";
        exit(0);
    }
}


/*
 * Handles the cmd args
 */
void handleArgs(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: ./lcs.out <filePath> \n";
        exit(0);
    }

    std::string current_exec_name = argv[0];
    std::vector<std::string> all_args;
    all_args.assign(argv + 1, argv + argc);

    filePath = all_args[0];
}

int mapper(char x){
    if (x == 'A') return 0;
    if (x == 'C') return 1;
    if (x == 'G') return 2;
    return 3;
}

/*
 * Execute the LCS
 */
Result executeLcs() {
    auto start = chrono::high_resolution_clock::now();

    int m = X.length(), n = Y.length();
    int L[2][n + 1];
    bool bi;

    for (int i = 0; i <= m; i++) {
        bi = i & 1;
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[bi][j] = 0;
            else if (X[i-1] == Y[j-1])
                L[bi][j] = L[1 - bi][j - 1] + 1;
            else
                L[bi][j] = max(L[1 - bi][j],
                               L[bi][j - 1]);
        }
    }

    auto stop = chrono::high_resolution_clock::now();
    double duration = duration_cast<milliseconds>(stop - start).count();

    Result result(std::to_string(L[bi][n]));
    result.setTimeElapsed(duration);

    return result;
}

/*
 * Execute the LCS
 */
Result executeLcs_v2() {
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
    for (int j = 0; j <= Y.size(); j++) currentRow[j] = 0;

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

/*
 * Write result to csv
 */
void writeCsv(const Result& result) {
    std::ofstream file;
    file.open("Outputs/output5.csv", std::ios_base::app);
    file << X.size() << "," << result.getLcs() << "," << result.getTimeElapsed() << "\n";
    file.close();
}

/*
 * Save the result of the execution to file
 */
void saveToFile(const Result& result){
    std::cout << "LCS(X,Y) = " << result.getLcs() << std::endl;
    std::cout << "took " << result.getTimeElapsed() << " seconds\n";
    writeCsv(result);
}

int main(int argc, char *argv[]) {
    handleArgs(argc, argv);
    readFile();

    Result result = executeLcs_v2();
    saveToFile(result);

    return 0;
}