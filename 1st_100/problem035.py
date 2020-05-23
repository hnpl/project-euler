# Question: https://projecteuler.net/problem=35

from sage.all import *

# Avoid numbers containing {0, 2, 4, 6, 8} as at least one of the rotations is divisible by 2.
# Avoid numbers containing {0, 5} as at least one of the rotations is divisible by 5.


N = 10**6

primes = prime_range(N)
primes = set(primes)

def is_circular_prime(x):
    n = len(x)
    result = True
    for i in range(n):
        x = x[-1] + x[:-1]
        if not int(x) in primes:
            result = False
    return result

result = 0

for i in primes:
    s = str(i)
    if any(digit in s for digit in '024568'):
        continue
    if is_circular_prime(s):
        result = result + 1

print(result)