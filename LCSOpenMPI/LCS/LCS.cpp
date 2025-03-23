//
// Created by marco on 15/07/22.
//

#include "LCS.h"

LCS::LCS(const std::string &x, const std::string &y) : X(x), Y(y) {}

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
