//
// Created by marco on 15/07/22.
//

#ifndef LCSOPENMPI_LCS_V1_H
#define LCSOPENMPI_LCS_V1_H

#include "../LCS.h"
#include "../Result.h"

class LCS_v1 : public LCS{
public:
    LCS_v1(const std::string &x, const std::string &y);

    ~LCS_v1() = default;

    Result computeLCS() override;
};


#endif //LCSOPENMPI_LCS_V1_H
