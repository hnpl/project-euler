# Question: https://projecteuler.net/problem=82

# Let's transpose the matrix. So the problem becomes, from any cell from the top row, find the shortest path to any cell in the bottom row, and the allowed movements are going leftward, going rightward, and going downward.

# The algorithm is the same as that of problem 81; however, instead of only calculating the moves from the right, we also calculate the moves from the left.

# For each row, it doesn't make sense to move rightward and then leftward (and vice versa).
# Therefore, each row only has at most one of {moving rightward, moving leftward}.
# Calculting the rightward moves is the same as problem 81.

# For each DP[i][j], we can reach that cell by going downward from DP[i-1][j], or (going downward from DP[i-1][j-n] and going rightward n times), or (going downward from DP[i-1][j+n] and going leftward n times.

import numpy as np

N = 80

old_DP = np.zeros(N+2, dtype = np.uint64)
DP     = np.zeros(N+2, dtype = np.uint64)
cost_table = np.zeros((N, N), dtype = np.uint64)

with open("inputs/p082_matrix.txt", "r") as f:
    # The only way to reach a tile in the first row is to move rightward from the top left tile
    for i, line in enumerate(f):
        for j, cost in enumerate(list(map(int, line.strip().split(",")))):
            cost_table[i][j] = cost

cost_table = np.transpose(cost_table)

for j in range(N):
    y = j + 1
    DP[y] = cost_table[0][j]

for i in range(1, N):
    old_DP, DP = DP, old_DP
    # DP_R: 
    # |
    # |-----> (downward then rightward) (this is from problem 81)
    # DP_L:
    #            |
    # <----------| (downward the leftward) (the opposite of the above)
    DP_L = np.zeros(N+2, dtype = np.uint64)
    DP_R = np.zeros(N+2, dtype = np.uint64)
    cost = cost_table[i]
    DP_R[0] = 99999999
    DP_L[-1] = 99999999
    for j, c in enumerate(cost):
        y = j + 1
        DP_R[y] = min(old_DP[y], DP_R[y-1]) + c
    for j in range(N-1, -1, -1):
        y = j + 1
        DP_L[y] = min(old_DP[y], DP_L[y+1]) + cost[j]
    for j in range(N):
        y = j + 1
        DP[y] = min(DP_L[y], DP_R[y])
print(min(DP[1:-1]))
