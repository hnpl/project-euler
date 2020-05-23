# Question: https://projecteuler.net/problem=587

from sage.all import *
from sage.symbolic.integration.integral import indefinite_integral

import time

x = var('x')
y = var('y')

S_blue = (1*1 - pi * (1/2)**2)/4
S_L2 = indefinite_integral(-sqrt((1/2)**2 - (x-1/2)**2) + 1/2, x)
S_L2_1 = S_L2(x=1/2)

def S_orange(n):
    slope = 1/n

    line = slope * x - y
    circle = (x-1/2)**2 + (y-1/2)**2 - (1/2)**2

    intersections = solve([line == 0, circle == 0], x, y, solution_dict = True)

    i = 0
    if intersections[0][x] > intersections[1][x]:
        i = 1

    intersection = (intersections[i][x], intersections[i][y])

    S_L = intersection[0] * intersection[1] / 2

    S_R = S_L2_1 - S_L2(x=intersection[0])    

    return S_L + S_R

ratio_cache = {}
def cached_ratio(n):
    if not n in ratio_cache:
        ratio_cache[n] = float(S_orange(n) / S_blue)
    return ratio_cache[n]
ratio = lambda n: cached_ratio(n)

def binary_search(lowerbound, upperbound, target):
    l = lowerbound
    r = upperbound
    while l < r:
        m = (l+r)//2
        if ratio(m) > target:
            l = m + 1
        else:
            r = m - 1
    if ratio(l) > target:
        l = l + 1
    return l

TARGET = 0.001
delta = 128
upperbound = 1
while ratio(upperbound) > 0.001:
    upperbound = upperbound + delta
ans = binary_search(upperbound - 128, upperbound, TARGET)
print(ans)
