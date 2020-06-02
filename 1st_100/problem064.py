# Question: https://projecteuler.net/problem=64

# https://doc.sagemath.org/html/en/reference/diophantine_approximation/sage/rings/continued_fraction.html

from sage.all import *

N = 10000

square_numbers = {i*i for i in range(int(sqrt(N)) + 1)}

ans = 0

var('n')

for i in range(2, N+1):
    if i in square_numbers:
        continue
    K = QuadraticField(i, 'n')
    period_length = len(continued_fraction(K('n')).period())
    if period_length % 2 == 1:
        ans = ans + 1

print(ans)
