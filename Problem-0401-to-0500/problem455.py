# Problem: https://projecteuler.net/problem=455

# Approach: see https://oeis.org/A165736

from sage.all import *

N = 10**6

if __name__ == "__main__":
    ans = 0

    R = IntegerModRing(10**9)

    for n in range(2, N+1):
        if n % 10 == 0:
            continue
        a, b = 0, 1
        while not a == b:
            a, b = b, R(n) ** b
        ans += lift(b)

    print(ans)


