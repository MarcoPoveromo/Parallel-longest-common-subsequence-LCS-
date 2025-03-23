#!/bin/bash

# Compile
files="main.cpp LCS/LCS.cpp LCS/LCS.h LCS/Result.cpp LCS/Result.h LCS/Impl/LCS_v1.cpp LCS/Impl/LCS_v1.h LCS/Impl/LCS_v2.cpp LCS/Impl/LCS_v2.h"
compileOptions="-fopenmp -std=c++11"
output="build/lcs.out"
g++ $compileOptions $files -o $output


# Init test variables
testFiles=()
numberThreads=(1 2 3 4 5 6 7 8)
lcsTypes=(0 1)
search_dir=Test

# Init the testFiles array
for fileName in "$search_dir"/*
do
  testFiles+=("$fileName")
done

# Run Tests
for f in "${testFiles[@]}"; do
  for n in "${numberThreads[@]}"; do
    for t in "${lcsTypes[@]}"; do
      echo "./$output $n $t $f"
      ./"$output" "$n" "$t" "$f"
    done
  done
done
