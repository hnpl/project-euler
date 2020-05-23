# Question: https://projecteuler.net/problem=81

import numpy as np

N = 80

old_DP = np.zeros(N+1, dtype = np.uint64)
DP     = np.zeros(N+1, dtype = np.uint64)

with open("inputs/p081_matrix.txt", "r") as f:
    line = f.readline()
    # The only way to reach a tile in the first row is to move rightward from the top left tile
    cost = map(int, line.strip().split(","))
    for j, c in enumerate(cost):
        y = j + 1
        DP[y] = DP[y-1] + c
    for line in f:
        old_DP, DP = DP, old_DP
        DP[0] = 99999999 # prevent the left most tiles from going from their left tiles (which are not in the matrix)
        cost = map(int, line.strip().split(","))
        for j, c in enumerate(cost):
            y = j + 1
            DP[y] = min(old_DP[y], DP[y-1]) + c
print(DP[-1])