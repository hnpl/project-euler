# Question: https://projecteuler.net/problem=133

# Wrote this after I learn about multiplicative order.

# R(10^n) = [10^(10^n) - 1] / 9
# If p|R(10^n), then p | [10^(10^n)-1]/9
# -> 9p | [10^(10^n)-1]
# -> 10^(10^n) = 1 (mod 9p)
# We know 10^0 = 1 (mod 9p)
# If we can find multiplicative order of 10 and 9p (https://en.wikipedia.org/wiki/Multiplicative_order), and suppose it is k, then we know:
#   - The modulo group of 9p has the period of k
#   - 10^(xk) = 1 (mod 9p) for some integer x
# For xk = 10^n for some n, both x and k must not be divisible by any number other than 2 and 5.

from sage.all import *

N = 100000

primes = []
with open('../inputs/primes_1e6.txt', 'r') as f:
    for line in f:
        prime = int(line)
        if prime > N:
            break
        primes.append(int(line)) 

x = Mod(10, 9)

ans = sum(primes)
for prime in primes:
    if prime == 2 or prime == 5: # those are not coprime with 10, and from the problem statement, we know that they are not divisible by any R(10^n).
        continue
    prime9 = 9 * prime
    k = Mod(10, prime9).multiplicative_order()
    while k % 2 == 0:
        k = k // 2
    while k % 5 == 0:
        k = k // 5
    if k == 1:
        ans = ans - prime

print(ans)