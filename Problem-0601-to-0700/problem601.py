# Question: https://projecteuler.net/problem=601

from math import factorial, floor, gcd

# Suppose we have streak(n) = k
# Then,
# 1 | n            -> 1 | n - 1
# 2 | n + 1        -> 2 | n - 1
# 3 | n + 2        -> 3 | n - 1
# ...
# k | n + k - 1    -> k | n - 1
# n + k not divisible by k + 1 -> n -1 is not divisible by k + 1

# So we need to find the number of n's such that, n - 1 is divisible by 1->k but not by k + 1
# which is the number of n's such that n-1 is divisible by 1->k but not by 1->k+1

# Since 1 < n < N, 0 < n - 1 < N - 1, or (n-1) in [1, N-2]
# number of n-1 that is divisible by 1->k: floor((N-2) / lcm(1,2,3,4,5...,k))
# number of n-1 that is divisible by 1->k+1: floor((N-2) / lcm(1,2,3,4,5,...,k,k+1))

def lcm(a, b):
    return a * b // gcd(a,b)

count = 0
LCM_k = 1
LCM_k_1 = 1
N = 1

for i in range(1, 32):
    LCM_k = LCM_k_1
    LCM_k_1 = lcm(LCM_k, i+1)
    N = N * 4
    count = count + (N - 2) // LCM_k - (N - 2) // LCM_k_1

print(count)
