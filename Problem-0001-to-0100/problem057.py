# Question: https://projecteuler.net/problem=57

from fractions import Fraction as f

N = 1000

ans = 0

n = f(1,1) + f(1,2)

for i in range(2, N+1):
    n = f(1,1) + 1/(1+n)
    if len(str(n.numerator)) > len(str(n.denominator)):
        ans = ans + 1

print(ans)

