# Question: https://projecteuler.net/problem=474

from math import log10
from collections import defaultdict

#N = 10**6
#M = 65432
N = 50
M = 123
#N = 12
#M = 12
MASK = 10**(int(log10(M)) + 1) # if the last 5 digits of X are 65432, then X mod (10**5) = 65432
MOD = 10**16 + 61

if __name__ == "__main__":
    ans = 0

    primes = []
    with open("../inputs/primes_1e6.txt", "r") as f:
        for p in f.readlines():
            next_prime = int(p.strip())
            if next_prime > N:
                break
            primes.append(next_prime)

    exp = {prime: 0 for prime in primes}
    for prime in primes:
        base = prime
        while base <= N:
            exp[prime] += N // base
            base = base * prime

    prev = {}
    curr = defaultdict(lambda: 0)
    curr[1] = 1

    for prime in primes:
        print(prime, len(curr))
        prev, curr = curr, defaultdict(lambda: 0)
        period_start = 0
        period_length = 0
        seen = {}
        base = 1
        exp_ = 0

        while exp_ <= exp[prime]:
            if base in seen:
                period_start = seen[base]
                period_length = exp_ - seen[base]
                break
            seen[base] = exp_
            base = (base * prime) % MASK
            exp_ = exp_ + 1

        mod_freq_map = defaultdict(lambda: 0)
        if period_length == 0:
            for mod, _ in seen.items():
                mod_freq_map[mod] = 1
        else:
            for mod, first_exp in seen.items():
                if first_exp < period_start:
                    mod_freq_map[mod] = 1
                else:
                    mod_freq_map[mod] = (exp[prime] - period_start) // period_length

        for prev_mod, prev_freq in prev.items():
            for mod, freq in mod_freq_map.items():
                next_mod = (prev_mod * mod) % MASK
                next_freq = (prev_freq * freq) % MOD
                curr[next_mod] = (curr[next_mod] + next_freq) % MOD


    print(curr[M])

