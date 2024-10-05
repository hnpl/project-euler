# Question: https://projecteuler.net/problem=23

"""
from math import sqrt

def is_abundant(n):
    sum = 1
    i = 1
    for i in range(2, int(sqrt(n))+1):
        if n % i == 0:
            sum = sum + i + (n//i)
    if i*i == n:
        sum = sum - i
    if sum > n:
        return True
    return False
"""

import numpy as np

N = 28123

divisor_sums = np.zeros(N+1, dtype = np.uint32)
for i in range(2, N):
    divisor_sums[i*2:N+1:i] += i

is_abundant = [True if divisor_sums[n] > n else False for n in range(0, N+1)]
sum_from_abundants = [False] * (N+1)
for i in range(1, N//2+1):
    if not is_abundant[i]:
        continue
    for j in range(i, N-i+1):
        if is_abundant[j]:
            sum_from_abundants[i+j] = True
ans = 0
for i in range(1, N+1):
    if sum_from_abundants[i]:
        ans = ans + i
sum1n = lambda n: n*(n+1)//2
print(sum1n(N) - ans)
