

#include "Result.h"

Result::Result(const std::string &lcs) : lcs(lcs) {}

const std::string &Result::getLcs() const {
    return lcs;
}

void Result::setLcs(const std::string &lcs) {
    this->lcs = lcs;
}

int Result::getLength() const {
    return length;
}

void Result::setLength(int length) {
    this->length = length;
}

double Result::getTimeElapsed() const {
    return timeElapsed;
}

void Result::setTimeElapsed(double timeElapsed) {
    this->timeElapsed = timeElapsed/1000;
}
