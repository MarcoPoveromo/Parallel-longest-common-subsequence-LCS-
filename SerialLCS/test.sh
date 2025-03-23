#!/bin/bash

# Compile
g++ -std=c++11 main.cpp Result.h Result.cpp -o build/lcs.o

# Init test variables
testFiles=()
search_dir=Test

# Init the testFiles array
for fileName in "$search_dir"/*; do
  testFiles+=("$fileName")
done

# Run Tests
for f in "${testFiles[@]}"; do
  echo "build/lcs.o $f"
  build/lcs.o "$f"
done

echo "Done"

