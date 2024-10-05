# Question: https://projecteuler.net/problem=191

import numpy as np

N = 30

# DP1[i] -> i^th tile is 'O'
# DP2[i] -> i^th tile is 'L'
# DP3[i] -> the last two tiles are 'OA' or 'LA'
# DP4[i] -> the last three tiles are 'OAA' or 'LAA'

# DP1[0][i] -> has 0 L's
# DP1[1][i] -> has 1 L's

DP1 = np.zeros((2, N+1), dtype = np.uint64) # '..........O'
DP2 = np.zeros((2, N+1), dtype = np.uint64) # '..........L'
DP3 = np.zeros((2, N+1), dtype = np.uint64) # '.....(O/L)A'
DP4 = np.zeros((2, N+1), dtype = np.uint64) # '....(O/L)AA'

DP1[0][0], DP1[0][1], DP1[0][2] = 1, 1, 2 # empty string, {O}, {OO, AO}
DP1[1][0], DP1[1][1], DP1[1][2] = 0, 0, 1 # empty, empty, {LO}

DP2[1][0], DP2[1][1], DP2[1][2] = 0, 1, 2 # empty, {L}, {OL, AL}

DP3[0][0], DP3[0][1], DP3[0][2] = 0, 1, 1 # empty, {A}, {OA}
DP3[1][0], DP3[1][1], DP3[1][2] = 0, 0, 1 # empty, empty, {LA}

DP4[0][0], DP4[0][1], DP4[0][2] = 0, 0, 1 # empty, empty, {AA}
DP4[1][0], DP4[1][1], DP4[1][2] = 0, 0, 0 # empty, empty, empty


for i in range(3, N+1):
    DP1[0][i] = DP1[0][i-1] + DP3[0][i-1] + DP4[0][i-1]
    DP2[0][i] = 0
    DP3[0][i] = DP1[0][i-1]
    DP4[0][i] = DP1[0][i-2]

for i in range(3, N+1):
    DP1[1][i] = DP1[1][i-1] + DP2[1][i-1] + DP3[1][i-1] + DP4[1][i-1]
    DP2[1][i] = DP1[0][i-1] + DP2[0][i-1] + DP3[0][i-1] + DP4[0][i-1]
    DP3[1][i] = DP1[1][i-1] + DP2[1][i-1]
    DP4[1][i] = DP1[1][i-2] + DP2[1][i-2]

result = DP1[0][N] + DP2[0][N] + DP3[0][N] + DP4[0][N] + DP1[1][N] + DP2[1][N] + DP3[1][N] + DP4[1][N]

print(result)

