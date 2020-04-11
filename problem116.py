# Question: https://projecteuler.net/problem=116

import numpy as np

N = 50

tile_length = [2,3,4]

DP = np.zeros((len(tile_length), N+1), dtype = np.uint64)


for i, n in enumerate(tile_length):
    for j in range(n): # if we have fewer than n tiles, the only solution is placing nothing
        DP[i][j] = 1
    for j in range(n, N+1):
        # placing nothing: DP[i][j-1]
        # placing an additional n-tile: DP[i][j-n]
        DP[i][j] = DP[i][j-1] + DP[i][j-n]

# note that we also count placing no tiles in DP, so we'll need to substract 1 from DP[i][N]
print(DP[0][N] - 1 + DP[1][N] -1 + DP[2][N] - 1)

assert(DP[0][5] - 1 == 7)
assert(DP[1][5] - 1 == 3)
assert(DP[2][5] - 1 == 2)
