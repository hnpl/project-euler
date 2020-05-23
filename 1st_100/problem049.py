# Question: https://projecteuler.net/problem=49

import itertools

primes = []

with open("inputs/primes_1e6.txt", "r") as f:
    for line in f:
        n = int(line.strip())
        if n >= 10000:
            break
        if n >= 1000:
            primes.append(n)

prime_set = set(primes)
checked = set()
found = False

for i in itertools.permutations(list('1478')):
    prime_set.discard(int(''.join(map(str, i))))

for prime in primes:
    if prime in checked:
        continue
    prime = str(prime)
    count = 0
    sol = set()
    for i in itertools.permutations(list(prime)):
        n = int(''.join(map(str, i)))
        if n in prime_set:
            sol.add(n)
        checked.add(n)
    if len(sol) >= 3:
        for i in itertools.combinations(list(sol), 3):
            i = sorted(i)
            if i[1] - i[0] == i[2] - i[1]:
                sol = i
                found = True
                break
    if found:
        break

print(''.join(map(str, sol)))
