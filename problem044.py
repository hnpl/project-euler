# Question: https://projecteuler.net/problem=44

N = 10000

pentagon_nums = [i*(3*i-1) for i in range(1, N+1)]

pentagon_num_set = set(pentagon_nums)

best_diff = N * (3*N - 1)

for i, a in enumerate(pentagon_nums):
    for b in pentagon_nums[i:]:
        if b - a > best_diff:
            break
        if (b - a) in pentagon_num_set and (b + a) in pentagon_num_set:
            best_diff = min(b-a, best_diff)

# we use n(3n-1), but the problem statement uses n(3n-1)/2
print(best_diff // 2)
