# Question: https://projecteuler.net/problem=114

N = 50

# DP1[i] -> ways of putting red blocks within i tiles where i^th tile is gray
# DP2[i] -> ways of putting red blocks within i tiles where i^th tile is red
# When the i^th is gray, the previous tile could be red or gray
# So, DP1[i] = DP1[i-1] + DP2[i-1]
# Patterns in DP1 and DP2 are distinct -> no overlap
# When the i^th is red, i-1 and i-2 must be red
# DP2[i-1] + R -> the last 4 tiles are RRRR
# DP1[i-3] + RRR -> the last 4 tiles are GRRR -> constructing new a red block
# -> no overlap
# why DP1[i-3]? GRR is not in DP1[i-1], so if we use DP1[i-1], we cannot construct GRRR
# So, DP2[i] = DP1[i-3] + DP2[i-1]

# Do DP1 and DP2 cover all cases? (The proof is not convincing at all ...)
# Define DP1[i] and DP2[i] as the numbers of ways putting red blocks in i tiles such that there no patterns GRRG
# Suppose we have DP1[i] and DP2[i] that are correct for i < N
# Consider DP1[N],
#   If there exists a valid pattern X of length N such that X[:-1] is not in DP1[N-1] and DP2[N-1] that ends with G
#   Obviously, X[:-1] is invalid
#   Since DP1[N-1] and DP2[N-1] are disjoint, DP1[N] = DP1[N-1] + DP2[N-1]
# Consider DP2[N],
#   Since DP2 ends with R, the last 3 tiles must be RRR
#   Case 1: last 4 tiles of DP2[N] is RRRR
#      If a valid X of length N such that X[:-1] is not in DP2[N-1], then X[:-1] must be in DP1[N-1], which means X[-2] = G (contradiction)
#   Case 2: last 4 tiles of DP2[N] is GRRR
#      If a valid X of length N such that X[:-3] is not in DP1[N-3], then X[:-3] must be invalid


DP1 = [0] * (N+1)
DP2 = [0] * (N+1)
DP1[0] = 1
DP1[1] = 1
DP1[2] = 1
DP2[0] = 0
DP2[1] = 0
DP2[2] = 0

for i in range(3, N+1):
    DP1[i] = DP1[i-1] + DP2[i-1]
    DP2[i] = DP1[i-3] + DP2[i-1]

print(DP1[N] + DP2[N])

assert(DP1[7] + DP2[7] == 17)

