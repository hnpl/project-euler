# Question: https://projecteuler.net/problem=124

# Brute-force
"""
from sage.all import *

nums = {}
N = 100000
for i in range(1, N+1):
    f = factor(i)
    p = 1
    for x, y in f:
        p = p * x
    nums[i] = p
l = sorted(nums, key = nums.get)
print(l[9999])
"""

# Non brute-force
import numpy as np
N = 100000
rads = np.ones(N+1, dtype = np.uint64)
for i in range(2, N):
    if rads[i] == 1:
        rads[i::i] *= i
# O(n log n)
print(np.argsort(rads, kind='stable')[10000]) # rads[0] = 1, we don't count 0
