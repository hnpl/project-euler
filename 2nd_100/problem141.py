# Problem: https://projecteuler.net/problem=141

"""
    . n = dq + r
    . WLOG, r < d < q (either this or r < q < d)
    . Since r, d, q is a geometric progression with a rational ratio, we have, for (a, b) such that gcd(a,b) = 1,
    and a > b
        . d = r * (a/b)
        . q = r * (a/b)^2
    . Since q is an integer, b^2 | r.
    . Let r = kb^2, then
        y^2 = n = b(k^2)(a^3) + kb^2
    . We have that 2 < a < 10**4; a>b; gcd(a,b) = 1; b(k^2)(a^3) + kb^2 < 10**12.
        We can iterate through all a, b, k to find n
"""

from math import gcd, sqrt

N = 10**12

def is_square(i):
    s = int(round(sqrt(i)))
    return s*s == i

if __name__ == "__main__":
    ans = 0
    pps = set()
    for a in range(2, 10**4):
        print(a)
        for b in range(1, a+1):
            if not gcd(a,b) == 1:
                continue
            k = 1
            while True:
                n = b * (k**2) * (a**3) + k * (b**2)
                if n >= N:
                    break
                if is_square(n):
                    pps.add(n)
                k += 1
    ans = sum(pps)
    print(ans)
