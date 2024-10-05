# Question: https://projecteuler.net/problem=72

# find all pairs (a,b) such that a < b <= d and gcd(a,b) = 1
# so, using Euler's totient function, we can get the number of such 'a' for a given 'b'

d = 10**6
sorted_primes = []
primes = sorted_primes
prime_set = set(primes)

with open("inputs/primes_1e6.txt", "r") as f:
    for line in f:
        sorted_primes.append(int(line.strip()))

invert_primes = [1 - 1/p for p in sorted_primes]
phi_s = [i for i in range(d+1)]

count = 0
for i, p in enumerate(primes):
    if p > d:
        break
    phi_s[p] = p - 1
    for b in range(p*2, d+1, p):
        phi_s[b] = phi_s[b] * invert_primes[i]
count = sum(phi_s) - phi_s[1]

"""
for b in range(2, d+1):
    phi = b * (1 - 1/b)
    p_i = 0

    while sorted_primes[p_i] <= b // 2:
        p = sorted_primes[p_i]
        if b % p == 0:
            bp = b // p
            if b % (p*p) == 0:
                phi = phi_s[bp] * p
            else:
                phi = p * invert_primes[p_i] * phi_s[bp]
            break
        p_i = p_i + 1
    phi_s[b] = int(phi)
    print(b, phi_s[b])
    count = count + int(phi)
"""
print(count)

