# Question: https://projecteuler.net/problem=101

from scipy.interpolate import lagrange
from numpy.polynomial.polynomial import polyval
import numpy as np

x = list(range(1,12))
y = polyval(x, [(-1)**i for i in range(11)])
#y = [i**3 for i in x]
sum = 0
k = 0
for i in range(1,11):
    polynomial = lagrange(x[:i], y[:i])
    sum = sum + polynomial(i+1)
print(sum)

