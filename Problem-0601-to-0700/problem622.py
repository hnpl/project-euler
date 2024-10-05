# Problem: https://projecteuler.net/problem=622

"""
According to https://en.wikipedia.org/wiki/Faro_shuffle, k perfect out-shuffles will lead to the original order of n cards if
    2^k = 1 (mod n - 1)

Since 2^k = 1 (mod n - 1) [this also means k is the multiplicative order of Mod(2, n-1)]
   -> 2^k-1 = 0 (mod n - 1)
So, (2^k-1) is divisible by n-1.

In this problem, we have k = 60.
So, we will check all divisors d of 2^60-1, and for each divisor, we check if multiplicative_order(2, d) = 60.
"""

N = 60

from sage.all import divisors, Mod

ans = 0

for divisor in divisors(2**N-1):
    if Mod(2, divisor).multiplicative_order() == N:
        ans = ans + (divisor + 1)

print(ans)