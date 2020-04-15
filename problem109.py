# Question: https://projecteuler.net/problem=109

import numpy as np

N = 100

S = list(range(1, 21))
D = [s*2 for s in S]
T = [s*3 for s in S]

S.append(25) # outer bull
D.append(50) # inner bull

ALL = []
ALL.extend(S)
ALL.extend(D)
ALL.extend(T)

DP = np.zeros((3,N+1), dtype = np.uint64)

# land on double to win
# throw 3 times

# DP[1][i]: number of ways to get the score of i with one throw
for a in ALL:
    DP[1][a] = DP[1][a] + 1

# DP[2][i]: number of ways to get the score of i with two throws
for i, first in enumerate(ALL):
    for second in ALL[i:]:
        if first + second < N:
            print(first+second)
            DP[2][first+second] = DP[2][first+second] + 1

result = 0
# number of ways to get the score of i with one double
for d in D:
    if d < N:
        result = result + 1

for i in range(1, N+1):
    for d in D:
        if i+d < N:
            result = result + DP[1][i] + DP[2][i]
            print(i, DP[1][i], DP[2][i])
print(result)

