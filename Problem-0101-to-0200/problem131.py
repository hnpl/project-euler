# Problem: https://projecteuler.net/problem=131

"""
    . Let n, p, a be positive integers.
    . Let cbtr(x) be the cube root of x.
    . n^3 + p*n^2 = a^3
    So, n^3 * (n+p) / n = a^3
    Take the cuberoot of the two sides, we have,
        n * cbrt(n+p)/cbrt(n) = a
        Since n and a are positive integers, then cbrt(n+p)/cbrt(n) must be rational.
        So, n+p and n must be cubes.
        Let n+p = X^3, n = Y^3 for positive integers X, Y.
        Then p = X^3 - Y^3 = (X-Y)(X^2+XY+Y^2).
        Note that X^2+XY+Y^2 = (X-Y)^2 + 3XY > X - Y.
        Since p is prime, there are 2 cases:
            Case 1: X-Y = P and X^2+XY+Y^2 = 1
                This means X-Y > X^2 + XY + Y^2 (contradiction)
            Case 2: X-Y = 1 and X^2+XY+Y^2 = P
        So, X-Y = 1, or X = Y + 1.
    Also, p = X^3 - Y^3 = (Y+1)^3 - Y^3
    So, we can iterate through all Y and find whether (Y+1)^3 - Y^3 is a prime that is smaller than 10^6.
"""

from sage.all import is_prime

N = 1e6


ans = 0

Y = 0

while True:
    Y = Y + 1
    X = Y + 1
    p = X**3 - Y**3
    if p > N:
        break
    if is_prime(p):
        ans = ans + 1

print(ans)
