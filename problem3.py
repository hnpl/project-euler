# Question: https://projecteuler.net/problem=3

import math

N = 600851475143

factor_upperbound = int(math.sqrt(N))

factor = 2

while N > factor_upperbound:
    while N % factor == 0:
        N = N // factor
    factor = factor + 1        

print(N)