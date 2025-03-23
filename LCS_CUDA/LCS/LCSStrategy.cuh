//
// Created by marco on 26/06/22.
//

#ifndef PARALLELLCS_LCSSTRATEGY_CUH
#define PARALLELLCS_LCSSTRATEGY_CUH


#include <string>
#include "Result.cuh"

class LCSStrategy {
protected:
    std::string X;
    std::string Y;
public:
    LCSStrategy(const std::string &x, const std::string &y);

    ~LCSStrategy() = default;

    const std::string &getX() const;

    void setX(const std::string &x);

    const std::string &getY() const;

    void setY(const std::string &y);

    virtual Result computeLCS() = 0;
};


#endif //PARALLELLCS_LCSSTRATEGY_CUH
