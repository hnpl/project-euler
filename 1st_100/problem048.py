# Question: https://projecteuler.net/problem=48

# find the largest key that is not bigger than the target
def binary_search(keys, target):
    i = 0
    j = len(keys) - 1
    m = (i + j) // 2
    while i <= j:
        m = (i + j) // 2
        if keys[m] < target:
            i = m + 1
        elif keys[m] > target:
            j = m - 1
        else:
            return m
    if keys[m] > target:
        return m - 1
    return m

N = 1000
sum = 0
M = 10**10

for i in range(1, N+1):
    cache = {1: i}
    j = 1
    m = i
    while j*2 < i:
        j = j * 2
        m = (m * m) % M
        cache[j] = m
    keys = sorted(list(cache.keys())) # 1, 2, 4, 8, ...
    while j < i:
        key = keys[binary_search(keys, i-j)]
        m = (m * cache[key]) % M
        j = j + key
    sum = (sum + m) % M

print(sum)
