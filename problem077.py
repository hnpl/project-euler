# Question: https://projecteuler.net/problem=77

# 1: 1
# 2: 1+1,       | 2
# 3: 1+1+1,     | 1+2,             | 3
# 4: 1+1+1+1,   | 1+1+2, 2+2       | 1+3,        | 4
# 5: 1+1+1+1+1, | 1+1+1+2, 1+2+2,  | 1+1+3, 2+3, | 1+4, | 5

import numpy as np

primes = []
with open('inputs/primes_1e6.txt', 'r') as f:
    primes = list(map(int, f.readlines()))

M = 1000 # avoiding overflow ...
N = 5000
# DP[j]: number of ways of writing the sum j
DP = np.ones(M+1, dtype = np.uint64) - 1
DP[0] = 1 # 0
#DP[1] = 1 # 1
#DP[2] = 2 # 2, 1+1

prev_prime = 0
found = False
for prime in primes: # ending in +prime
    for j in range(prime, M+1):
        DP[j] = DP[j] + DP[j - prime]
    for i in range(prev_prime, min(prime+1, M+1)):
        if DP[i] - 1 > N:
            found = True
            break
    if found:
        break
    prev_prime = prime

print(i)
