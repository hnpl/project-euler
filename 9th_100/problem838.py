# Question: https://projecteuler.net/problem=838

# . We can reduce this problem to a linear programming problem.
# . Let S be the set of numbers in range [3, N] whose 3 as the least significant digit.
#   Let g: N -> set() that g(p) is a set containing prime factors of p.
#   Let T = union(f(p) for all p in S), i.e. T contains all prime factors of all elements in S.
# . We need to find a set Q that is a subset of T such that, for every element x in S, there exists an element y in Q such that x is divisible by y.
#   This is true because we want to find the log of the minimum product P such that P is not a coprime with every element in S.
#   This means P is a product of a set of prime factors that minimizing its product.
#   log(P) = sum(log(prime_factor) for all prime_factor)
# . Let v[p] be a binary variable indicating whether we put p into the final product.
#   Objective function: min[sum(log(p) * v[p] for all p in T)]
#   Constraints:
#       for all n in S:
#           sum(v[p] for all p in g(p)) >= 1
#
# . E.g, consider N = 40.
#   Then,
#      3 = 3
#     13 = 13
#     23 = 23
#     33 = 3 * 11
#   So S = {3, 13, 23, 33} and T = {3, 11, 13, 23}
#   Objective function: min[v[3] * log(3) + v[11] * log(11) + v[13] * log(13) + v[23] * log(23)]
#   Constraints:
#       v[3] >= 1            // for 3
#       v[13] >= 1           // for 13
#       v[23] >= 1           // for 23
#       v[3] + v[11] >= 1    // for 33

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

primes_f = set()
for k in range(3, N+1, 10):
    for prime in prime_factors[k]:
        primes_f.add(prime)
primes_f_index_map = {prime: index for index, prime in enumerate(primes_f)}

# https://doc.sagemath.org/html/en/reference/numerical/sage/numerical/mip.html
# can solve faster(?) via COIN-OR backend that uses multi-threading
# https://github.com/sagemath/sage-numerical-backends-coin
solver = MixedIntegerLinearProgram(maximization=False, solver="GLPK")
v = solver.new_variable(binary=True)
for k in range(3, N+1, 10):
    solver.add_constraint(solver.sum(v[primes_f_index_map[prime]] for prime in prime_factors[k]) >= 1)
solver.set_objective(solver.sum(v[primes_f_index_map[prime]] * n(log(prime)) for prime in primes_f))
result = solver.solve()
print(result)
