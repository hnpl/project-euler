# https://oeis.org/A064775

from sage.all import *

def n_primes(lowerbound, upperbound):
    return prime_pi(upperbound) - prime_pi(lowerbound-1)

N = 10**10
sqrt_N = int(sqrt(N))
ans = N

for prime in prime_range(1, sqrt_N+1):
    ans -= prime - 1 + 1
for quotient in range(1, N // sqrt_N):
    ans -= quotient * n_primes(N//(quotient+1)+1, N//quotient)

second_way = """
for q in range(1, sqrt_N):
    ans -= n_primes(q+1, N//q)
ans -= prime_pi(sqrt_N)
"""

print(ans)
