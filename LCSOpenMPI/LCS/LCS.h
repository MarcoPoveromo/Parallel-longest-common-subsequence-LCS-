//
// Created by marco on 15/07/22.
//

#ifndef LCSOPENMPI_LCS_H
#define LCSOPENMPI_LCS_H

#include <string>
#include "Result.h"

class LCS {
protected:
    std::string X;
    std::string Y;
public:
    LCS(const std::string &x, const std::string &y);

    ~LCS() = default;

    const std::string &getX() const;

    void setX(const std::string &x);

    const std::string &getY() const;

    void setY(const std::string &y);

    virtual Result computeLCS() = 0;
};


#endif //LCSOPENMPI_LCS_H
