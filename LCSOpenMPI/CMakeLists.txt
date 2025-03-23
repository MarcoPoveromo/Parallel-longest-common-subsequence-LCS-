cmake_minimum_required(VERSION 3.22)
project(LCSOpenMPI)

set(CMAKE_CXX_STANDARD 11)

add_executable(LCSOpenMPI main.cpp LCS/LCS.cu LCS/LCS.cuh LCS/LCS.cpp LCS/LCS.h LCS/Result.cpp LCS/Result.h LCS/Impl/LCS_v2.cpp LCS/Impl/LCS_v2.h LCS/Impl/LCS_v1.cpp LCS/Impl/LCS_v1.h)
