# Question: https://projecteuler.net/problem=197

from decimal import *
from math import floor
from fractions import Fraction

getcontext().prec = 10

f = lambda x: int(floor(Decimal(2)**(Decimal(30.403243784) - x**2))) * Decimal(10**-9)

ans = Decimal(-1)
prev_ans = Decimal(1)
sum = Decimal(-1)
prev_sum = Decimal(0)
tol = Decimal(10**-10)

while (abs(sum - prev_sum) > tol):
    prev_ans = ans
    prev_sum = sum
    ans = f(prev_ans)
    sum = ans + prev_ans

print(sum)
