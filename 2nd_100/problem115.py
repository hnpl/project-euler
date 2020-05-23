# Question: https://projecteuler.net/problem=115

M = 50
T = 10**6

DP1 = []
DP2 = []

for i in range(M):
    DP1.append(1)
    DP2.append(0)

while DP1[-1] + DP2[-1] <= T:
    DP1_i = DP1[-1] + DP2[-1]
    DP2_i = DP1[-M] + DP2[-1]
    DP1.append(DP1_i)
    DP2.append(DP2_i)

print(len(DP1) - 1)

# ---
# Even better recursive formula
# Let DP[N] = DP1[N] + DP2[N]
# We have, DP1[N] = DP1[N-1] + DP2[N-1] = DP[N-1] -> DP1[N] = DP[N-1]
#                                                and DP2[N-1] = DP[N-1] - DP1[N-1] = DP[N-1] - DP[N-2]
# We have DP2[N] = DP1[N-M] + DP2[N-1] = DP[N-M-1] + (DP[N-1] - DP[N-2]) 
# So, DP[N] = DP1[N] + DP2[N] = DP[N-1] + (DP[N-M-1] + DP[N-1] - DP[N-2]) = 2 * DP[N-1] - DP[N-2] + DP[N-M-1]

DP = []
for i in range(M):
    DP.append(1)
DP.append(2)

while DP[-1] <= T:
    DP.append(2 * DP[-1] - DP[-2] + DP[-M-1])
print(len(DP) - 1)

assert(DP[-1] == DP1[-1] + DP2[-1])
