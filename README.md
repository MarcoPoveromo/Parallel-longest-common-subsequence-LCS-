# Parallel longest common subsequence (LCS)
In computational biology it is still a research topic to find strategies for
optimizing sequence alignment. The LCS is referred to the longest common
subsequence, and is an effective method to solve this problem. Generally,
the approach for the LCS is the dynamic programming, and the score table
can be filled in a parallel diagonal or changing the data dependency. The
goal of the following work is to present and analize the use of different
models related to parallel programming in solving the LCS. A sperimental
comparison between OpenMP, MPI, CUDA will be shown, on two different
computer setups. Analyzes will be carried out on the executions of two
variants of the algorithm with different parameters. It will then be shown
how the CUDA approach will be the most efficient.

## Docs
**[📄 Show Paper](http://MarcoPoveromo.github.io/Parallel-longest-common-subsequence-LCS-/docs/SCP___Longest_common_subsequence.pdf)**

