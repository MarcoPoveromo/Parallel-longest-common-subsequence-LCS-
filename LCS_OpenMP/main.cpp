#include <iostream>
#include "LCS/LCS.h"
#include "LCS/Impl/LCS_v1.h"
#include "LCS/Impl/LCS_v2.h"

#include <bits/stdc++.h>

using namespace std;

/*
 * Global variables
 */
int lcsType;
string X, Y;
string filePath;
int nThreads;

/*
 * Generate a random sequence
 */
std::string randomString(int length) {
    char basis[] = {'A', 'C', 'G', 'T'};
    char *buffer = (char*) malloc (length + 1);
    if (buffer==nullptr) exit(1);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,3); // distribution in range [1, 3]

    buffer[length] = '\0';

    for (int i = 0; i < length; i++) {
        buffer[i] = basis[dist(rng)];
    }

    return buffer;
}

/*
 * Read the sequences file
 */
void readFile() {
    fstream newfile;
    newfile.open(filePath,ios::in);
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
    if (argc != 4) {
        cout << "Usage: ./lcs.out <nThreads> <lcsType> <filePath> \n";
        exit(0);
    }

    std::string current_exec_name = argv[0];
    std::vector<std::string> all_args;
    all_args.assign(argv + 1, argv + argc);

    nThreads = std::stoi(all_args[0]);
    lcsType = std::stoi(all_args[1]);
    filePath = all_args[2];
}

/*
 * Execute the chosen LCS
 */
Result executeLcs() {
    LCS *lcs;
    if(lcsType == 0) lcs = new LCS_v1(X, Y);
    else lcs = new LCS_v2(X, Y);


    lcs->setNThreads(nThreads);
    Result result = lcs->computeLCS();

    return result;
}

/*
 * Write result to csv
 */
void writeCsv(const Result& result) {
    std::ofstream file;
    file.open("Outputs/output3.csv", std::ios_base::app);
    file << nThreads << "," << X.size() << "," << lcsType << "," << result.getLcs() << "," << result.getTimeElapsed() << "\n";
    file.close();
}

/*
 * Save the result of the execution to file
 */
void saveToFile(const Result& result){
    if(lcsType == 0) {
        std::cout << "LCS(X,Y) = " << result.getLcs() << std::endl;
        std::cout << "v1 took " << result.getTimeElapsed() << " seconds\n";
    } else {
        std::cout << "LCS(X,Y) = " << result.getLcs() << std::endl;
        std::cout << "v2 took " << result.getTimeElapsed() << " seconds\n";
    }
    writeCsv(result);
}

int main(int argc, char *argv[]) {
    handleArgs(argc, argv);
    readFile();

    Result result = executeLcs();
    saveToFile(result);

    return 0;
}