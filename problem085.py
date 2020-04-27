# Question: https://projecteuler.net/problem=85

# Number of rectangles inside a rectangle of size N x M
#   of size 1x1: N x M
#   of size 1x2: N x (M-1)
#   of size 1x3: N x (M-2)
#   ...
#   of size i, j: (N-i+1) x (M-j+1)
#
#   Total number of rectangles:
#      sum_{i:[1, N]} (
#              sum_{j:[1, M]} (
#                          [(N - i + 1)*(M - j + 1)] ) )
#   =  sum_{i:[1, N]} (
#              (N - i + 1) * sum_{j: [1, M]} (
#                                         (M - j + 1) ) )
#   =  sum_{i:[1, N]} ( (N - i + 1) * (M^2 - M(M+1)/2 + M) )
#   =  (N^2 - N(N+1)/2 + N) * (M^2 - M(M+1)/2 + M)
#   =  N(N+1)M(M+1)/4
# We precalculate f(i) = i(i+1)/2 for some i, then search a pair f(i)f(j) that minimizes abs(2,000,000 - f(i)f(j))
# To avoid division operations, we precalcuate f(i) = i(i+1) for some i, then search a pair of f(i)f(j) that minimizes abs(8,000,000 -f(i)f(j)

from math import sqrt

target = 8000000
N = int(sqrt(target))
f = {i:i*(i+1) for i in range(N+1)}

best = target
best_i = 0
best_j = 0

for i in range(N+1):
    for j in range(N+1):
        n_rects = f[i] * f[j]
        diff = abs(n_rects - target)
        if diff < best:
            best, best_i, best_j = diff, i, j
        if n_rects > target:
            break

print(best_i * best_j)
