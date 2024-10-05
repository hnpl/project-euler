# Question: https://projecteuler.net/problem=267

# For a fixed variable 'f' such that 0 <= f <= 1, the equation (1+2f)^W * (1-f)^(N-W) goes down then goes up after a certain W as (1+2f)*W outgrows (1-f)^(N-W) as W increases.
# The probability of having W wins after N flips: C(N, W) / 2^N (note that f does not appear).
# Also note that 'f' is continuous, while W is discrete.
# We can perform a grid search on 0 <= f <= 1 and 0 <= W <= N.
# Since we know the behavior of (1+2f)^W * (1-f)^(N-W) for a fixed 'f', we fix 'f' and then search the first W that (1+2f)^W * (1-f)^(N-W) >= 10^9.
# We find the smallest W such that there exists 'f' such that (1+2f)^W * (1-f)^(N-W) >= 10^9.
# Also, the probability of winning at least 10^9 units of money is sum_{min_W <= W <= N}(C(1000, W)/2^N).

from scipy.special import comb as C
import numpy as np

N = 1000

def grid_search():
    # (1+2f)^W * (1-f)^(N-W) >= 10^9
    min_W = 1000
    step = 0.01
    grid = np.zeros((101, N+1), dtype = np.float64)
    for f in range(1, 101):
        df = step * f
        for W in range(min_W+1):
            grid[f][W] = ((1+2*df)**W) * ((1-df)**(N-W))
            if grid[f][W] >= 10**9:
                min_W = W
                break
    return W

min_W = grid_search()

# https://www.wolframalpha.com/input/?i=max+%281%2B2*f%29%5E431+*+%281-f%29%5E%281000-431%29
# -> with W = 431, we have [max (1+2*f)^431 * (1-f)^(1000-431) < 10^9].
# so min_W = 432.

ans = 1
for i in range(min_W):
    ans = ans - C(N, i)/2**1000

print("{:.12f}".format(ans))
