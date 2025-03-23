//
// Created by marco on 15/07/22.
//

#ifndef LCSOPENMPI_RESULT_H
#define LCSOPENMPI_RESULT_H

#include <string>

class Result {
protected:
    std::string lcs;
    int length;
    double timeElapsed;
public:
    Result(const std::string &lcs);

    ~Result() = default;

    const std::string &getLcs() const;

    void setLcs(const std::string &lcs);

    int getLength() const;

    void setLength(int length);

    double getTimeElapsed() const;

    void setTimeElapsed(double timeElapsed);
};

#endif //LCSOPENMPI_RESULT_H
