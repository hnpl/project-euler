# Question: https://projecteuler.net/problem=838

# Same as problem_838.py, except that:
# . S: set of number in [3, N] with the least significant digit of 3.
# . T: a set of prime factors appearing in all elements in S.
# . Since there are numbers in S are prime numbers, those prime numbers are part of the final product.
#   When we include those prime numbers to the final product, those prime numbers and their multiples are not co-prime with the final product.
#   Thus, in the linear programming formulation, we can ignore those prime numbers and their multiples as we always include those prime numbers in the final product.
#   Consequently, we have smaller S, T, and v.

from sage.all import *

def get_prime_factors(k, primes):
    prime_factors = [set() for i in range(k+1)]
    for prime in primes:
        for prime_multiple in range(prime, k, prime):
            prime_factors[prime_multiple].add(prime)
    return prime_factors

N = 10**6

primes = set(prime_range(3, N))
prime_factors = get_prime_factors(N, primes)

S_unoptimized = {k for k in range(3, N+1, 10)}

T = set()
primes_S_intersection = []
S_optimizing_out = set()

for k in S_unoptimized:
    if k in primes:
        primes_S_intersection.append(k)

for k in primes_S_intersection:
    for t in range(k, N+1, k):
        S_optimizing_out.add(t)

S = S_unoptimized - S_optimizing_out

for k in S:
    for prime in prime_factors[k]:
        T.add(prime)

T_index_map = {prime: index for index, prime in enumerate(T)}

# https://doc.sagemath.org/html/en/reference/numerical/sage/numerical/mip.html
# can solve faster(?) via COIN-OR backend that uses multi-threading
# https://github.com/sagemath/sage-numerical-backends-coin
solver = MixedIntegerLinearProgram(maximization=False, solver="GLPK")
v = solver.new_variable(binary=True)
for k in S:
    solver.add_constraint(solver.sum(v[T_index_map[prime]] for prime in prime_factors[k]) >= 1)
solver.set_objective(solver.sum(v[T_index_map[prime]] * n(log(prime)) for prime in T))
result = solver.solve()
for p in primes_S_intersection:
    result += n(log(p))
print(result)
