cmake_minimum_required(VERSION 3.22)
project(LCS_CUDA CUDA)

set(CMAKE_CUDA_STANDARD 11)

add_executable(LCS_CUDA main.cu LCS/Result.cu LCS/Result.cuh LCS/LCSStrategy.cu LCS/LCSStrategy.cuh LCS/Implementations/LCS_v1.cu LCS/Implementations/LCS_v1.cuh LCS/Implementations/LCS_v2.cu LCS/Implementations/LCS_v2.cuh cuda_launch_config.cu)

# https://stackoverflow.com/questions/24238937/how-to-change-a-compiler-flag-for-just-one-executable-in-cmake
target_link_libraries(LCS_CUDA -lm -lcudart -lcublas -lcuda -fopenmp -L/usr/local/lib -Wl,-rpath -Wl,/usr/local/lib -Wl,--enable-new-dtags -lmpi -lopen-rte -lopen-pal -ldl -lnsl -lutil)

# https://forums.developer.nvidia.com/t/passing-flags-to-nvcc-via-cmake/75768/7
target_compile_options(LCS_CUDA PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
-Xcompiler -fopenmp;
-I/usr/local/include;
-I/usr/local/include/openmpi;
>)


set_target_properties(LCS_CUDA PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
