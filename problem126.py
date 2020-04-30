# https://projecteuler.net/problem=126

import numpy as np

N = 1000

f_delta_blocks = lambda a, b, c, n: 2*(a*b + b*c + c*a) + 4*(n-1)*(a + b + c) + 4*(n-1)*(n-2)
g = f_delta_blocks

max_g = 2**16

freq = np.ones(max_g, dtype = np.uint16) - 1

# assume a >= b >= c
a = 1
while g(a, 1, 1, 1) < max_g:
    b = 1
    print(a)
    while g(a, b, 1, 1) < max_g and b <= a:
        c = 1
        while g(a, b, c, 1) < max_g and c <= b:
            n = 1
            delta = g(a, b, c, 1)
            while delta < max_g:
                freq[delta] = freq[delta] + 1
                n = n + 1
                delta = g(a, b, c, n)
            c = c + 1
        b = b + 1
    a = a + 1

for i in range(max_g):
    if freq[i] == N:
        break

print(i)