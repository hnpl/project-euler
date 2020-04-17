# Question: https://projecteuler.net/problem=187

from sage.all import *

N = 10**8

primes = prime_range(N)

# find the largest prime that is not bigger than the target
def binary_search(primes, target):
    i = 0
    j = len(primes) - 1
    m = (i + j) // 2
    while i <= j:
        m = (i + j) // 2
        if primes[m] < target:
            i = m + 1
        elif primes[m] > target:
            j = m - 1
        else:
            return m
    if primes[m] > target:
        return m - 1
    return m

n = 0
m = binary_search(primes, sqrt(10**8))

for i, prime in enumerate(primes[:m+1]):
    factor = (N-1) // prime
    n = n + binary_search(primes, factor) - i + 1
print(n)
