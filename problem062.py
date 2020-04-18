# Question: https://projecteuler.net/problem=62

# Hash a cube as "abcdefghij" where each of the characters represents a digit such that
# a -> number of 0's
# b -> number of 1's
# ...
# So if x and y are each other's permutation then they will have the same hash.

from collections import Counter, defaultdict

freq = defaultdict(lambda: 0)
x_to_x_cube_map = {}

def to_hash(x):
    hash = 0
    counter = Counter(str(x**3))
    for i in '0123456789':
        hash = hash * 10 + counter[i]
    return hash

n = 0
while True:
    hash_n = to_hash(n)
    freq[hash_n] = freq[hash_n] + 1
    if not hash_n in x_to_x_cube_map:
        x_to_x_cube_map[hash_n] = n
    if freq[hash_n] == 5:
        break
    n = n + 1
print(x_to_x_cube_map[hash_n]**3)