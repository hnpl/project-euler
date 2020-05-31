# Question: https://projecteuler.net/problem=80

from decimal import *
from math import sqrt

getcontext().prec = 110

N = 100
square_numbers = {i*i for i in range(int(sqrt(N)+1))}

ans = 0

for i in range(2, N):
    if i in square_numbers:
        continue
    dec = str(Decimal(i).sqrt())
    dec = dec.replace('.', '')
    dec = dec[:N]
    ans = ans + sum(map(int, dec))
print(ans)
