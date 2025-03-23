#include <iostream>
#include <mpi.h>
#include <random>
#include "LCS/LCS.h"
#include "LCS/Result.h"
#include "LCS/Impl/LCS_v2.h"
#include "LCS/Impl/LCS_v1.h"
#include <fstream>

#define MASTER 0

using namespace std;

/*
 * Global variables for main
 */
int procRank, procSize;
int lcsType;
string filePath;
int lenX, lenY;
pair<string, string> X_Y;
std::string X;
std::string Y;

/*
 * Generates a random genome sequence in ACGT
 */
std::string randomString(int length) {
    char basis[] = {'A', 'C', 'G', 'T'};
    char *buffer = (char*) malloc (length + 1);
    if (buffer==nullptr) exit(1);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,3); // distribution in range [0, 3]

    buffer[length] = '\0';

    for (int i = 0; i < length; i++) {
        buffer[i] = basis[dist(rng)];
    }

    return buffer;
}

/*
 * Read the sequences file
 */
pair<string, string> readFile(std::string path) {
    fstream newfile;
    newfile.open(path,ios::in);
    if (newfile.is_open()){
        string tp;
        getline(newfile, tp); // Reject the first line
        getline(newfile, tp); // Get X
        X_Y.first = string(tp);
        getline(newfile, tp); // Get Y
        X_Y.second = string(tp);
        newfile.close();
    } else {
        cout << "File not found\n";
        exit(0);
    }

    return X_Y;
}

/*
 * Broadcast the strings lenght
 */
void init(string filePath) {
    // Pass X Y around ranks
    if(procRank == MASTER) {
        X_Y = readFile(filePath);
        lenX = X_Y.first.size();
        lenY = X_Y.second.size();
    }
    MPI_Bcast(&lenX, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    MPI_Bcast(&lenY, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    if(procRank == 0){
        X = X_Y.first;
        Y = X_Y.second;
    }
    X.resize(lenX);
    Y.resize(lenY);
}

/*
 * Handles the cmd args
 */
void handleArgs(int argc, char *argv[]) {
    if (argc != 3) {
        if(procRank == MASTER) cout << "Usage: ./lcs.exe <lcsType> <filePath> \n";
        MPI_Finalize();
        exit(0);
    }

    std::string current_exec_name = argv[0];
    std::vector<std::string> all_args;
    all_args.assign(argv + 1, argv + argc);

    lcsType = std::stoi(all_args[0]);
    filePath = all_args[1];
}

/*
 * Write result to csv
 */
void writeCsv(const Result& result) {
    std::ofstream file;
    file.open("Outputs/output3.csv", std::ios_base::app);
    file << procSize << "," << lenX << "," << lcsType << "," << result.getTimeElapsed() << "\n";
    file.close();
}

/*
 * Save the result of the execution to file
 */
void saveToFile(const Result& result){
    if(lcsType == 0) {
        if(procRank == MASTER) {
            std::cout << "LCS(X,Y) = " << result.getLcs() << std::endl;
            std::cout << "v1 took " << result.getTimeElapsed() << " seconds\n";

            writeCsv(result);
        }
    } else {
        if(procRank == procSize - 1) {
            std::cout << "LCS(X,Y) = " << result.getLcs() << std::endl;
            std::cout << "v2 took " << result.getTimeElapsed() << " seconds\n";

            writeCsv(result);
        }
    }
}

/*
 * Execute the chosen LCS
 */
Result executeLcs() {
    LCS *lcs;
    if(lcsType == MASTER) lcs = new LCS_v1(X, Y);
    else lcs = new LCS_v2(X, Y);

    Result result = lcs->computeLCS();

    return result;
}

/*
 * Main
 */
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);

    handleArgs(argc, argv);
    init(filePath);
    Result result = executeLcs();
    saveToFile(result);

    MPI_Finalize();

    return 0;
}
