#!/bin/bash

# Compile
files="main.cpp LCS/LCS.cpp LCS/LCS.h LCS/Result.cpp LCS/Result.h LCS/Impl/LCS_v2.cpp LCS/Impl/LCS_v2.h LCS/Impl/LCS_v1.cpp LCS/Impl/LCS_v1.h"
compileOptions="-std=gnu++11"
output="build/lcs.out"
mpic++ $compileOptions $files -o $output

# Init test variables
testFiles=()
numberProcesses=(1 2 3 4)
lcsTypes=(0 1)
search_dir=Test

# Init the testFiles array
for fileName in "$search_dir"/*
do
  testFiles+=("$fileName")
done

# Run Tests
for f in "${testFiles[@]}"; do
  for n in "${numberProcesses[@]}"; do
    for t in "${lcsTypes[@]}"; do
      echo "mpirun -np $n $output $t $f"
      mpirun -np "$n" $output "$t" "$f"
    done
  done
done


