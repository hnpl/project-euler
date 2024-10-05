# Question: https://projecteuler.net/problem=132

# R(10^9) = [10^(10^9) - 1] / 9
# If p|R(10^9), then p | [10^(10^9)-1]/9
# -> 9p | [10^(10^9)-1]
# -> 10^(10^9) = 1 (mod 9p)
# We know 10^0 = 1 (mod 9p)
# If we can find multiplicative order of 10 and 9p (https://en.wikipedia.org/wiki/Multiplicative_order), and suppose it is k, then we know:
#   - The modulo group of 9p has the period of k
#   - 10^(xk) = 1 (mod 9p) for some integer x
# For xk = 10^9, both x and k must not be divisible by any number other than 2 and 5.

from sage.all import *

N = 100000

ans = 0
count = 0
prime = 5 # consider all primes > 5 (2 and 5 are coprime with 10, so we cannot find multilicative_order of Mod(10, p))

while count < 40:
    prime = next_prime(prime)
    prime9 = 9 * prime
    k = Mod(10, prime9).multiplicative_order()
    if 10**9 % k == 0:
        ans = ans + prime
        count = count + 1

print(ans)