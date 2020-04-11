# Question: https://projecteuler.net/problem=24

import itertools

i = itertools.permutations(list('0123456789'))

for j in range(10**6-1):
    next(i)

print(''.join(next(i)))
