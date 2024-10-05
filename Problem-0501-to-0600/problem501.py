# Question: https://projecteuler.net/problem=501

from sage.all import *

N = 10**12

primes = list(prime_range(int(sqrt(N)+1)))

ans = 0

cache = {}
def cached_prime_pi(n):
    if not n in cache:
        cache[n] = prime_pi(n)
    return cache[n]

# n = a^7
a_upperbound = int(N**(1/7))
ans = ans + cached_prime_pi(a_upperbound)

# n = a^3 * b
for a in primes:
    a3 = a**3
    if a3 >= N:
        break
    b_upperbound = N // a3
    if b_upperbound <= 1:
        break
    ans = ans + cached_prime_pi(b_upperbound)
    if b_upperbound >= a:
        ans = ans - 1 # eliminate the case a == b

# n = a * b * c (a < b < c)
for i_a, a in enumerate(primes):
    a3 = a**3
    if a3 >= N:
        break
    for i_b, b in enumerate(primes[i_a+1:], start=i_a+1):
        ab = a*b
        if ab*b >= N:
            break
        c_lowerbound = b
        c_upperbound = N / ab
        if c_upperbound < c_lowerbound:
            break
        ans = ans + cached_prime_pi(c_upperbound) - cached_prime_pi(c_lowerbound)
    
print(ans)