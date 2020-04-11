with open('inputs/p067_triangle.txt', 'r') as f:
    triangle = f.readlines()

N = len(triangle)

DP = (N + 2) * [0]
old_DP = (N + 2) * [0]

for n, row in enumerate(triangle):
    nums = [int(num) for num in row.strip().split(' ')]
    nums = [0] + nums + [0]
    old_DP, DP = DP, old_DP # pointer swapping
    for i in range(1, n+2):
        DP[i] = max(old_DP[i-1], old_DP[i]) + nums[i]

assert(DP[0] == 0)
assert(DP[-1] == 0)
print(max(DP))

