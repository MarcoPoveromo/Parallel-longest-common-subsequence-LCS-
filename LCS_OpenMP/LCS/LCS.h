//
// Created by marco on 15/07/22.
//

#ifndef LCSOPENMP_LCS_H
#define LCSOPENMP_LCS_H

#include <string>
#include "Result.h"

class LCS {
protected:
    std::string X;
    std::string Y;
protected:
    int nThreads;
public:
    LCS(const std::string &x, const std::string &y);

    ~LCS() = default;

    const std::string &getX() const;

    void setX(const std::string &x);

    const std::string &getY() const;

    void setY(const std::string &y);

    int getNThreads() const;

    void setNThreads(int nThreads);

    virtual Result computeLCS() = 0;

};


#endif //LCSOPENMP_LCS_H
