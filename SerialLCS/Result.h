//
// Created by marco on 26/06/22.
//

#ifndef PARALLELLCS_RESULT_CUH
#define PARALLELLCS_RESULT_CUH

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


#endif //PARALLELLCS_RESULT_CUH
