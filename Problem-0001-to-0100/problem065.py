# Question: https://projecteuler.net/problem=65
# Recursion formula for continued fractions: https://faculty.math.illinois.edu/~hildebr/453.spring11/nt-notes6.pdf | section 6.3, prop 6.5

import numpy as np

N = 100

a = [1] * (N+1)
a[0] = 2

for i, j in enumerate(range(2, N, 3)):
    a[j] = (i+1) * 2

p = [a[0], a[1] * a[0] + 1] #p[1], p[2]
#q = [1, a[1]]

for i in range(2, len(a)-1):
    p.append(a[i] * p[-1] + p[-2])
    #q.append(a[i] * q[-1] + q[-2])
digits = list(str(p[-1]))
print(sum(map(int, digits)))
