# Question: https://projecteuler.net/problem=193

# Finding squarefree number is hard. I think it requires computing prime numbers up to 2^50.
# We can use ans = N - non-squarefree.
# Let k be a squarefree number, and suppose k has m prime factors.
# -> k^2 = p_1^2 * p_2^2 * p3^2 * ... * p_m^2
# Let x be any number -> p_i^2 | x*k^2.
#
# Inclusion-exclusion principle, use Möbius function.
#   n_nonsquarefree = sum(number of multiples of k * Möbius(k))
#     Möbius(k) =  1 if m is even
#               = -1 if m is odd
#   e.g. n += N / p^2
#        n -= N / (p^2 * q^2)
#        n += N / (p^2 * q^2 * r^2)

import numpy as np

N = 2**50
sqrt_N = int(np.sqrt(N))

is_prime = np.ones(sqrt_N+1, np.bool)
is_prime[0] = False
is_prime[1] = False
for i in range(2, sqrt_N+1):
    if not is_prime[i]:
        continue
    is_prime[2*i::i] = False

primes = np.where(is_prime == True)[0]

n_prime_factors = np.zeros(sqrt_N+1, np.int32)
mask = np.ones(sqrt_N+1, np.int32)

for prime in primes:
    n_prime_factors[prime::prime] += 1
    q = prime*prime
    while q < sqrt_N:
        mask[q::q] = 0
        q = q*prime

M = n_prime_factors

M[n_prime_factors % 2 == 1] = 1
M[n_prime_factors % 2 == 0] = -1

M = np.multiply(n_prime_factors, mask)

ans = N - np.sum(N//np.power(np.arange(2, sqrt_N+1),2) * M[2:])

print(ans)

