from sage.all import *
from bisect import bisect_right

def reverse_digits(n):
    return int("".join(list(reversed(list(str(n))))), 10)

steps = [int(ceil((sqrt((10**n_digits))))) for n_digits in range(0, 16)]
ans = set()
for n_digits in range(2, 15, 2):
    prime_lowerbound = steps[n_digits]
    prime_upperbound = steps[n_digits+1]
    primes = prime_range(prime_lowerbound, prime_upperbound)
    prime_squares = {p*p for p in primes}
    n = len(primes)
    for i in range(n):
        p1 = primes[i]
        p1_square = p1*p1
        if p1_square in ans:
            continue
        p2_square = reverse_digits(p1_square)
        if p2_square in prime_squares and p1_square != p2_square:
            ans.add(p1_square)
            ans.add(p2_square)
    print(n_digits, len(ans))

ans = list(sorted(list(ans)))
#print(len(ans))
print(sum(ans[:50]))
