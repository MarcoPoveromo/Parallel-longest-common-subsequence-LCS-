cmake_minimum_required(VERSION 3.22)
project(LCS_OpenMP)

set(CMAKE_CXX_STANDARD 11)

add_executable(LCS_OpenMP main.cpp LCS/LCS.cpp LCS/LCS.h LCS/Result.cpp LCS/Result.h LCS/Impl/LCS_v1.cpp LCS/Impl/LCS_v1.h LCS/Impl/LCS_v2.cpp LCS/Impl/LCS_v2.h)

# https://stackoverflow.com/questions/24238937/how-to-change-a-compiler-flag-for-just-one-executable-in-cmake
target_link_libraries(LCS_OpenMP -fopenmp)

# https://forums.developer.nvidia.com/t/passing-flags-to-nvcc-via-cmake/75768/7
target_compile_options(LCS_OpenMP PRIVATE $<$<COMPILE_LANGUAGE:CPP>:
        -Xcompiler -fopenmp;
        -I/usr/local/include;
        -I/usr/local/include/openmpi;
        >)