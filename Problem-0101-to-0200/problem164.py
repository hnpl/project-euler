# Problem: https://projecteuler.net/problem=164

"""
    Let DP[n,a,b] indicate the number of such a number described in the problem statement
                  that has n-digit and a and b being the last two digits.
    For a fixed b and c, DP[n,b,c] = sum_{a | a + b + c <= 9}(DP[n-1,a,b]).
"""

import numpy as np

N = 20

ans = 0

DP = np.zeros((N+1, 10, 10), dtype = np.int64)

for i in range(1, 10):
    DP[1, 0, i] = 1

for n in range(2, N+1):
    for a in range(10):
        for b in range(10):
            for c in range(9-a-b+1):
                DP[n, b, c] = DP[n, b, c] + DP[n-1, a, b]


for i in range(10):
    for j in range(10):
        ans = ans + DP[N, i, j]

print(ans)

