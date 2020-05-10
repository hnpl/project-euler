# Question: https://projecteuler.net/problem=53

from sage.all import binomial
N = 100
count = 0
for i in range(23, N+1):
    for j in range(2, i//2 + 2):
        if binomial(i, j) > 10**6:
            count = count + i - 2*j + 1
            break

print(count)
