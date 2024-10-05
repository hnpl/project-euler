# Problem: https://projecteuler.net/problem=346

"""
Repunits of n digits of base b has the form of (in base 10) r(b, n) = 1 + b + b^2 + ... + b^(n-2) + b^(n-1)
                                                                    = (b^n-1)/(b-1)

For any positive integer b,
    n = 1: r(b, 1) = 1
    n = 2: r(b, 2) = 1 + b
    n = 3: r(b, 3) = 1 + b + b^2
    ...

Observations: . 1 is a repunit of all bases.
              . a positive integer k in a repunit of base (k - 1)
                    Why? r(k-1, 2) = k
              . r(b, m) >= b^2 for all m >= 3

From the observations, we have that:
    All strong repunits k in the range [1, N] iff k = r(b, n) for some b <= sqrt(N) and n >= 3.
    Why?
        (=>)
        . Let k be a strong repunit in the range [2, N].
            -> k is a repunit of base k - 1
        . Now consider a base b where k is also a repunit where b != k - 1 (we considered it above).
            . If b > sqrt(N):
                r(b, 1) = 1 != k
                r(b, 2) = 1 + b != k
                r(b, 3) = 1 + b + b^2 > b^2 > N^2 (contradicts k <= N).
            -> b <= sqrt(N) (and obviously, n >= 3).
        (<=) this direction is trivial.
"""

from math import sqrt

N = 10**12
SQRT_N = int(sqrt(N))

ans = 0

ans += 1 # 1 is a repunit of all bases

strong_repunits = set()

for base in range(2, SQRT_N+1):
    n = base ** 3
    r = (n-1)//(base-1)
    while r < N:
        strong_repunits.add(r)
        n = n * base
        r = (n-1)//(base-1)

ans += sum(strong_repunits)

print(ans)

