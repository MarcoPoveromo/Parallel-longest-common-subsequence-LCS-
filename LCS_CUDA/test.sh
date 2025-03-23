#!/bin/bash

# Compile
/home/marco/Applications/clion-2022.1/bin/cmake/linux/bin/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=/home/marco/Applications/clion-2022.1/bin/ninja/linux/ninja -G Ninja -S /home/marco/CLionProjects/LCS_CUDA -B /home/marco/CLionProjects/LCS_CUDA/cmake-build-debug
/home/marco/Applications/clion-2022.1/bin/cmake/linux/bin/cmake --build /home/marco/CLionProjects/LCS_CUDA/cmake-build-debug --target LCS_CUDA

# Init test variables
testFiles=()
lcsTypes=(0 1)
search_dir=Test

# Init the testFiles array
for fileName in "$search_dir"/*; do
  testFiles+=("$fileName")
done

# Run Tests
for f in "${testFiles[@]}"; do
    for t in "${lcsTypes[@]}"; do
      echo "cmake-build-debug/LCS_CUDA $t $f"
      cmake-build-debug/LCS_CUDA "$t" "$f"
    done
done


