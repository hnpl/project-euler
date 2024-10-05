# Question: https://projecteuler.net/problem=31

import numpy as np

coin_val = [1, 2, 5, 10, 20, 50, 100, 200]
M = 200
N = len(coin_val)

DP = np.zeros((N, M+1), dtype = np.int)
DP[:,0] = DP[:,0] + 1
for num_coins in range(N):
    coins = coin_val[:num_coins+1]
    for total in range(M+1):
        for i, coin in enumerate(coins):
            if total - coin >= 0:
                DP[num_coins][total] = DP[num_coins][total] + DP[i][total - coin]
print(DP[-1][M])
