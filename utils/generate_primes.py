import math
import numpy as np

N = 10**6

is_prime = np.ones((N+1), dtype = np.bool)
is_prime[0] = False
is_prime[1] = False
for i in range(2, int(math.sqrt(N)) + 1):
    if not is_prime[i]:
        continue
    is_prime[2*i::i] = False

primes = np.where(is_prime==True)[0]

assert(len(primes) == 78498)

with open("inputs/primes_1e6.txt", "w") as f:
    for p in sorted(primes):
        f.write("{}\n".format(p))
