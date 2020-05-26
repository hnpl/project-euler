# Question: https://projecteuler.net/problem=130

# R(n) = [10^n - 1] / 9
# If p|R(n), then p | [10^n - 1]/9
# -> 9p | [10^n-1]
# -> 10^n = 1 (mod 9p)
# We know 10^0 = 1 (mod 9p)
# If we can find multiplicative order of 10 and 9p (https://en.wikipedia.org/wiki/Multiplicative_order), and suppose it is k, then we know:
#   - The modulo group of 9p has the period of k

from sage.all import *

N = 100000

ans = 0
count = 0
n = 5 # consider all primes > 5 (2 and 5 are coprime with 10, so we cannot find multilicative_order of Mod(10, p))

while count < 25:
    n = n + 1
    if n % 2 == 0 or n % 5 == 0 or is_prime(n): # GCD(n, 10) = 1
        continue
    n9 = 9 * n
    k = Mod(10, n9).multiplicative_order()
    if (n-1) % k == 0:
        ans = ans + n
        count = count + 1

print(ans)
