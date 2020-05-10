# Question: https://projecteuler.net/problem=33

from fractions import Fraction as F

r = set()

def truncate(d, n):
    d = set(list(str(d)))
    n = set(list(str(n)))
    i = d&n
    if len(i) == 1:
        d = d - i
        n = n - i
        i = int(next(iter(i)))
        d = int(next(iter(d)))
        n = int(next(iter(n)))
        return d, n, d > 0 and i > 0
    return 0, 0, False

for i in range(1, 10):
    for j in range(1, i):
        denominator = i
        numerator = j
        m = 1
        while denominator < 10 or numerator < 10:
            m = m + 1
            denominator = i * m
            numerator = j * m
        while denominator < 100 and numerator < 100:
            d, n, valid = truncate(denominator, numerator)
            if valid and F(n, d) == F(numerator, denominator):
                r.add(F(numerator, denominator))
            m = m + 1
            denominator = i * m
            numerator = j * m

f = F(1,1)
for i in r:
    f = f * i
print(f)
