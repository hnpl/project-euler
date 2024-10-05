# Question: https://projecteuler.net/problem=278

# This is the coin problem: https://en.wikipedia.org/wiki/Coin_problem
# For this specific case, f(pq, pr, qr) = lcm(pq, pr) + lcm(pq, qr) - pq - pr - qr
#                                       = pqr + pqr - pq - pr - qr
#                                       = 2pqr - pq - pr - qr
# This formula is obtained via working out the examples ;)

from sage.all import *

N = 5000

if __name__ == "__main__":
    ans = 0
    primes = prime_range(N)
    for i in range(len(primes)):
        p = primes[i]
        for j in range(i+1, len(primes)):
            q = primes[j]
            for k in range(j+1, len(primes)):
                r = primes[k]
                ans += 2*p*q*r - p*q - p*r - q*r
    print(ans)
