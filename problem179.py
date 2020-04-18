# Question: https://projecteuler.net/problem=179

import numpy as np
from math import sqrt
N = 10**7
n_divisors = np.zeros(N, dtype = np.uint64)
n_divisors = n_divisors + 2 # one and itself

for i in range(2, int(sqrt(N))):
    print(i)
    #for j in range(i, N, i):
    #    n_divisors[j] = n_divisors[j] + 2 # j and i/j
    n_divisors[i*i:N:i] += 2 # n_divisors[1*i -> (i-1)*i] are already calculated by i/j where j < i/j

for i in range(2, int(sqrt(N))):
    n_divisors[i*i] = n_divisors[i*i] - 1 # square numbers have i, j such that j = i/j

count = 0
for i in range(2, N-1):
    if n_divisors[i] == n_divisors[i+1]:
        count = count + 1

print(count)

