# Question: https://projecteuler.net/problem=345

# https://docs.scipy.org/doc/scipy-0.18.1/reference/generated/scipy.optimize.linear_sum_assignment.html
# This function was made to solve this problem ...

from scipy.optimize import linear_sum_assignment
import numpy as np

M = np.loadtxt('inputs/p345_matrix.txt', dtype = np.int64)
M = -M # the objective function of linear_sum_assignment is minimizing
coor = linear_sum_assignment(M)
print(-np.sum(M[coor]))