# Question: https://projecteuler.net/problem=117

import numpy as np 

N = 50
tile_length = [2, 3, 4]

DP = np.zeros((N+1), dtype = np.uint64)

DP[0] = 1
DP[1] = 1
DP[2] = 2

for i in range(3, N+1):
    # putting no tiles -> DP[i-1]
    # putting N-tile -> DP[i-N]
    DP[i] = DP[i-1] + sum([DP[i-n] for n in tile_length if i-n >= 0])
print(DP[N])
