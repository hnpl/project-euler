# Problem: https://projecteuler.net/problem=135

"""
    Let x = a + 2b
        y = a +  b
        z = a
    So, x^2 - y^2 - z^2 = n
     => (a+b) * (-a + 3b) = n
    Let X = a+b, Y = -a+3b, so n=XY and X > 0, which means Y > 0, and X <= n and Y <= n.
    We can iterate through all possible X and Y and count the number of times XY appears.
    Note that a and b are integers, so we need to check whether (X,Y) produces integers a and b.
    We have, b = (X+Y)/4 and a = (3X-Y)/4.
    Therefore, X+Y must be divisible by 4, and 3X-Y must be divisible by 4, and X+Y > 0 and 3X-Y > 0.
"""

from collections import defaultdict

N = 1000000
M = 10

ans = 0

freq = defaultdict(lambda: 0)

for X in range(1, N+1):
    for Y in range(1, 3*X):
        if X*Y > N:
            break
        if ((X+Y)%4 == 0) and ((3*X-Y)%4==0):
            freq[X*Y] += 1

for key, val in freq.items():
    if val == M:
        ans = ans + 1

print(ans) 

