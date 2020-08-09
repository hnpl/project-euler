# Problem: https://projecteuler.net/problem=286

"""
    . At each distance x, there are two possibilities:
        1. P(scoring at distance x) = 1 - x/q
        2. P(not scoring at distance x) = x/q
    . We can use dynamic programming/memoization to calculate the probability of scoring exactly 20 points:
        DP[x][j] = DP[x-1][j-1] * (1 - x/q) + DP[x-1][j] * x/q
        where DP[x][j] -> the number of ways of scoring exactly j points after trying to score at distances from 1 to x.
    OR we can find a close form function of calculating the probability of scoring exactly 20 points:
        Let f(q) = product_{k from 1 to 50} [k/q + (1-k/q) * x]
                 = (1/q + (1-1/q) * x) * (2/q + (1-2/q) * x) * ... * (50/q + (1-50/q) * x)
        Then the probability of scoring exactly 20 points is the coefficient of x^20.
        For simplicity, let p = 1/q.
    . Either way, we use binary search to find q within (50, +inf)  (or p in (0, 50)) such that the probability = 0.02
"""

from sage.all import *
from decimal import *

getcontext().prec = 12

N = 50


if __name__ == "__main__":
    var('x')
    var('p')

    f = 1
    for i in range(1, N+1):
        f = f * (i*p + (1-i*p)*x)
    
    g = f.expand()
    h = g.coefficient(x**20)

    target = 0.02
    lowerbound = Rational('0')       # switch to Rational() for higher precision
    upperbound = Rational('1/50')
    count = 0                        # count the number of time we have the correct value
    stop_threshold = 50              # this is the stopping criteria
                                     # if the count of the number of time we have the correct value is higher than stop_threshold, the algorithm will stop
    tol = 10**-12                    # the algorithm will see values in [target-tol, target+tol] to be the same as target

    # binary search for p
    while True:
        m = (lowerbound + upperbound) / 2
        new_val = h(p=m)
        if abs(new_val - target) < tol:
            count = count + 1
            if count == stop_threshold:
                break
        else:
            count = 0
        # this is a monotically increasing polynomial for p in [0, 1/50]
        # so we'll move the upperbound to the middle if the current value is bigger than the target value
        if new_val > target:
            upperbound = m
        else:
            lowerbound = m
    q = float(1/m)

    print("{:.10f}".format(q))