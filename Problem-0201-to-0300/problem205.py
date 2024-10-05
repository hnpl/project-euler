# Question: https://projecteuler.net/problem=205

import numpy as np

# Distribution of sum of 9 4-sided
DP1 = np.zeros((10, 37), dtype = np.uint64)
# Distribution of sum of 6 6-sided
DP2 = np.zeros((7, 37), dtype = np.uint64)

DP1[0][0] = 1
DP2[0][0] = 1

for i in range(1, 10):
    for total in range(1, 37):
        for val in range(1, 5):
            if total - val >= 0:
                DP1[i][total] = DP1[i][total] + DP1[i-1][total-val]

for i in range(1, 7):
    for total in range(1, 37):
        for val in range(1, 7):
            if total - val >= 0:
                DP2[i][total] = DP2[i][total] + DP2[i-1][total-val]

sum1 = np.array(DP1[-1][:]) # sum1[i] -> number of cases where sum is less than i
sum2 = np.array(DP2[-1][:])

for i in range(2, 37):
    sum1[i] = sum1[i-1] + DP1[-1][i]

for i in range(2, 37):
    sum2[i] = sum2[i-1] + DP2[-1][i]

n_DP1_gt_DP2 = 0
for i in range(2, 37):
    n_DP1_gt_DP2 = n_DP1_gt_DP2 + DP1[-1][i] * sum2[i-1]

print(n_DP1_gt_DP2/ (sum(DP1[-1] * sum(DP2[-1]))))
