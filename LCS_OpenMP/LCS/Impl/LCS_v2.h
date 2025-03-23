//
// Created by marco on 15/07/22.
//

#ifndef LCSOPENMP_LCS_V2_H
#define LCSOPENMP_LCS_V2_H

#include "../LCS.h"
#include "../Result.h"

class LCS_v2 : public LCS {
public:
    LCS_v2(const std::string &x, const std::string &y);

    ~LCS_v2() = default;

    Result computeLCS() override;
};

#endif //LCSOPENMP_LCS_V2_H
