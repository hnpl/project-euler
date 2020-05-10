# Question: https://projecteuler.net/problem=76


# 1: 1
# 2: 1+1,       | 2
# 3: 1+1+1,     | 1+2,             | 3
# 4: 1+1+1+1,   | 1+1+2, 2+2       | 1+3,        | 4
# 5: 1+1+1+1+1, | 1+1+1+2, 1+2+2,  | 1+1+3, 2+3, | 1+4, | 5

import numpy as np

N = 100
# DP[j]: number of ways of writing the sum j
DP = np.ones(N+1, dtype = np.uint64) - 1
DP[0] = 1 # 0
#DP[1] = 1 # 1
#DP[2] = 2 # 2, 1+1

for i in range(1, N+1): # ending in +i
    for j in range(i, N+1):
        DP[j] = DP[j] + DP[j - i]

print(DP[-1]-1)
