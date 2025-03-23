
#include <omp.h>
#include "LCS.h"

LCS::LCS(const std::string &x, const std::string &y) : X(x), Y(y) {
    setNThreads(8);
}

const std::string &LCS::getX() const {
    return X;
}

void LCS::setX(const std::string &x) {
    X = x;
}

const std::string &LCS::getY() const {
    return Y;
}

void LCS::setY(const std::string &y) {
    Y = y;
}

int LCS::getNThreads() const {
    return nThreads;
}

void LCS::setNThreads(int nThreads) {
    this->nThreads = nThreads;

    // Set number of threads
    omp_set_dynamic(0);
    omp_set_num_threads(nThreads);
}
