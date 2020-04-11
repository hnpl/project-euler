# Question: https://projecteuler.net/problem=5

import numpy as np

N = 20

prime_factors = [2, 3, 5, 7, 11, 13, 17, 19]
highest_powers = [0] * (N+1)

for X in range(2, N+1):
    powers = [0] * (N+1)
    for prime in prime_factors:
        if prime > X:
            break
        while X % prime == 0:
            X = X // prime
            powers[prime] = powers[prime] + 1
    highest_powers = np.maximum(highest_powers, powers)

m = [np.power(x, y) for x, y in enumerate(highest_powers)]
print(np.prod(m))

