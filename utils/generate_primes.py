import math

N = 10**6

primes = {i*2 + 1 for i in range(1, N//2)}
primes.add(2)

for i in range(3, int(math.sqrt(N)) + 1):
    if not i in primes:
        continue
    for j in range(i*2, N+1, i):
        primes.discard(j)

assert(len(primes) == 78498)

with open("inputs/primes_1e6.txt", "w") as f:
    for p in sorted(primes):
        f.write("{}\n".format(p))
