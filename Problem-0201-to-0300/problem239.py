# Question: https://projecteuler.net/problem=239

"""
 . There are 25 primes between 1 and 100. So, if we want exactly 22 primes displaced, there are always 3 primes that are in the correct places.
 . The number of arrangements having exactly 3 primes at their correct place is,
    C(25, 3) * n_derangements
  where C(25, 3) is the number of ways of choosing 3 primes to be fixed in their correct positions,
    and n_derangements is the number of ways of constructing an arrangments of the other 97 numbers such that, none of each of the other 22 primes would be at its correct place. (we don't care about non-primes)
 . To find n_derangements, we use the inclusion-exclusion principle.
 . Let F(m) be number of arrangments that at least m primes would be at correct positions.
     F(m) = C(22, m) * factorial(97-m)
   where C(22, m) is the number of ways of choosing m primes to be fixed at their correct positions,
     and factorial(97-m) is the number of ways of arranging the other 97-m numbers.
 . We have, n_derangements = F(0) - F(1) + F(2) - ... (Principle of inclusion and exclusion)
"""

from sage.all import *

N = 100
N_PRIMES = len(prime_range(N))
N_DISPLACED_PRIMES = 22
N_FIXED_PRIMES = N_PRIMES - N_DISPLACED_PRIMES

if __name__ == "__main__":
    ans = binomial(N_PRIMES, N_FIXED_PRIMES)

    # Principle of inclusion and exclusion
    n_remaining_positions = N - N_FIXED_PRIMES
    n_derangements = 0 # number of ways of rearranging the other 97 numbers so that none of the displaced primes are in their correct position.
    for k in range(N_DISPLACED_PRIMES + 1):
        n_ways_having_at_least_k_primes_inplaced = factorial(n_remaining_positions - k) # fix the k primes, place other numbers randomly
        n_tuples_of_k_primes = binomial(22, k)
        if k % 2 == 1:
            n_derangements -= n_ways_having_at_least_k_primes_inplaced * n_tuples_of_k_primes
        else:
            n_derangements += n_ways_having_at_least_k_primes_inplaced * n_tuples_of_k_primes

    ans *= n_derangements

    ans = ans / factorial(N)

    print(float(ans))
