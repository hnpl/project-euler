# Question: https://projecteuler.net/problem=235

# https://en.wikipedia.org/wiki/Arithmetico%E2%80%93geometric_sequence
"""
Arithmetico–geometric sequence definition: 
    Each term is defined as: t_n = [a + (n-1)d]br^(n-1)
    Sum of the terms:
        S_n = sum_{k=1..n} = (ab - (a+nd)br^n)/(1-r) + dbr(1-r^n)/(1-r)^2

We modify the u(k) term to fit the above definition.
u(k) = (900-3k)r^(k-1) = (300 - k) 3r^(k-1) = (299 - (k-1)) 3r^(k-1)
So, a = 299
    d = -1
    b = 3
Then we use Newton's method to find where S_n + 600000000000 = 0 [[won't do this, can't control tolerence < 1e-16 with scipy.optimize.newton]].
Use binary search to find the result. By trial and error, we know that the answer lies within 1 and 2.
"""

from decimal import *

getcontext().prec = 60

a = Decimal(299)
d = Decimal(-1)
b = Decimal(3)
S_5000 = Decimal(-600000000000)

def f(r, n = 5000):
    r = Decimal(r)
    return (a*b - (a+n*d)*b*(r**n))/(1-r) + d*b*r*(1-(r**n))/(1-r)**2 - S_5000

max_iters = 100
tol = 1e-20
l = Decimal(-100)
r = Decimal(100)
fm = 1
target = Decimal(0)
iter = 0
while (fm > tol or fm < -tol) and max_iters > iter:
    iter = iter + 1
    m = (l + r)/2
    fm = f(m)
    if fm > target:
        l = m
    else:
        r = m
print("{:.12f}".format(m))

