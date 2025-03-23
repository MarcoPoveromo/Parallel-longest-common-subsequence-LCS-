//
// Created by marco on 26/06/22.
//

#include "LCSStrategy.cuh"

LCSStrategy::LCSStrategy(const std::string &x, const std::string &y) : X(x), Y(y) {}

const std::string &LCSStrategy::getX() const {
    return X;
}

void LCSStrategy::setX(const std::string &x) {
    X = x;
}

const std::string &LCSStrategy::getY() const {
    return Y;
}

void LCSStrategy::setY(const std::string &y) {
    Y = y;
}


