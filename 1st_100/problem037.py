# Question: https://projecteuler.net/problem=37

primes = set()
with open('../inputs/primes_1e6.txt', 'r') as f:
    for p in f:
        primes.add(int(p.strip()))

def check(n):
    ns = str(n)
    # left
    for i in range(1, len(ns)):
        if not int(ns[i:]) in primes:
            return False

    # right
    for i in range(1, len(ns)):
        if not int(ns[:i]) in primes:
            return False

    return True

N = 11 + 4 # also count 2,3,5,7
n = 0
ans = 0
for prime in primes:
    if prime in primes and check(prime):
        n = n + 1
        ans = ans + prime
    if n == N:
        break

print(ans - sum([2,3,5,7]))
