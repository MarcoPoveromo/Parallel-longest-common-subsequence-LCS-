//
// Created by marco on 18/07/22.
//

#ifndef UNTITLED6_LCS_V1_CUH
#define UNTITLED6_LCS_V1_CUH

#include "../LCSStrategy.cuh"

class LCS_v1 : public LCSStrategy {
protected:
    dim3 nBlocks;
    dim3 nThreadsPerBlock;
public:
    LCS_v1(const std::string &x, const std::string &y, const dim3 &nBlocks, const dim3 &nThreadsPerBlock);

    LCS_v1(const std::string &x, const std::string &y);

    const dim3 &getNBlocks() const;

    void setNBlocks(const dim3 &nBlocks);

    const dim3 &getNThreadsPerBlock() const;

    void setNThreadsPerBlock(const dim3 &nThreadsPerBlock);

    Result computeLCS() override;

    Result computeLCS_try();
};

#endif //UNTITLED6_LCS_V1_CUH
