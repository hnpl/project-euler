# Question: https://projecteuler.net/problem=148

# The first few lines of the Pascal's Triangle show that, there's a pattern of location of zeros
# This sounds something something cellular automaton https://mathworld.wolfram.com/ElementaryCellularAutomaton.html
# Looks like rule 146 or rule 154

# The code does not have anything to do with automata theory

# TODO: explain the code

"""
R = IntegerModRing(7)
for i in range(70):
    s = []
    for j in range(i+1):
        s.append(R(binomial(i, j)))
    print(s)
"""

"""
[1]
[1, 1]
[1, 2, 1]
[1, 3, 3, 1]
[1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1] <- L1
[1, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 2, 6, 6, 2, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 2, 1, 5, 1, 2, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 2, 3, 6, 6, 3, 2, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 2, 5, 2, 5, 2, 5, 2, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 3, 6, 3, 0, 0, 0, 0, 3, 6, 3, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 3, 2, 2, 3, 0, 0, 0, 3, 2, 2, 3, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 3, 5, 4, 5, 3, 0, 0, 3, 5, 4, 5, 3, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 3, 1, 2, 2, 1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 3, 4, 3, 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 4, 1, 4, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 4, 1, 4, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 4, 5, 5, 4, 0, 0, 0, 6, 4, 4, 6, 0, 0, 0, 4, 5, 5, 4, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 4, 2, 3, 2, 4, 0, 0, 6, 3, 1, 3, 6, 0, 0, 4, 2, 3, 2, 4, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 4, 6, 5, 5, 6, 4, 0, 6, 2, 4, 4, 2, 6, 0, 4, 6, 5, 5, 6, 4, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 4, 3, 4, 3, 4, 3, 4, 6, 1, 6, 1, 6, 1, 6, 4, 3, 4, 3, 4, 3, 4, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 5, 3, 5, 0, 0, 0, 0, 3, 6, 3, 0, 0, 0, 0, 3, 6, 3, 0, 0, 0, 0, 5, 3, 5, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 5, 1, 1, 5, 0, 0, 0, 3, 2, 2, 3, 0, 0, 0, 3, 2, 2, 3, 0, 0, 0, 5, 1, 1, 5, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 5, 6, 2, 6, 5, 0, 0, 3, 5, 4, 5, 3, 0, 0, 3, 5, 4, 5, 3, 0, 0, 5, 6, 2, 6, 5, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 5, 4, 1, 1, 4, 5, 0, 3, 1, 2, 2, 1, 3, 0, 3, 1, 2, 2, 1, 3, 0, 5, 4, 1, 1, 4, 5, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 5, 2, 5, 2, 5, 2, 5, 3, 4, 3, 4, 3, 4, 3, 3, 4, 3, 4, 3, 4, 3, 5, 2, 5, 2, 5, 2, 5, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 6, 5, 6, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 6, 4, 4, 6, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 6, 4, 4, 6, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 6, 4, 4, 6, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 6, 3, 1, 3, 6, 0, 0, 1, 4, 6, 4, 1, 0, 0, 6, 3, 1, 3, 6, 0, 0, 1, 4, 6, 4, 1, 0, 0, 6, 3, 1, 3, 6, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 6, 2, 4, 4, 2, 6, 0, 1, 5, 3, 3, 5, 1, 0, 6, 2, 4, 4, 2, 6, 0, 1, 5, 3, 3, 5, 1, 0, 6, 2, 4, 4, 2, 6, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1, 6, 1] <- L2
[1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 1, 5, 3, 3, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 3, 3, 5, 1, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 1, 6, 1, 6, 1, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 1, 6, 1, 6, 1, 1, 6, 1, 6, 1, 6, 1]
[1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 1]
[1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 1, 1]
[1, 2, 1, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 2, 4, 2, 0, 0, 0, 0, 1, 2, 1]
[1, 3, 3, 1, 0, 0, 0, 2, 6, 6, 2, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 1, 0, 0, 0, 2, 6, 6, 2, 0, 0, 0, 1, 3, 3, 1]
[1, 4, 6, 4, 1, 0, 0, 2, 1, 5, 1, 2, 0, 0, 1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 4, 6, 4, 1, 0, 0, 2, 1, 5, 1, 2, 0, 0, 1, 4, 6, 4, 1]
[1, 5, 3, 3, 5, 1, 0, 2, 3, 6, 6, 3, 2, 0, 1, 5, 3, 3, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 3, 3, 5, 1, 0, 2, 3, 6, 6, 3, 2, 0, 1, 5, 3, 3, 5, 1]
[1, 6, 1, 6, 1, 6, 1, 2, 5, 2, 5, 2, 5, 2, 1, 6, 1, 6, 1, 6, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 6, 1, 6, 1, 6, 1, 2, 5, 2, 5, 2, 5, 2, 1, 6, 1, 6, 1, 6, 1]
"""

def binary_search(array, target):
    l = 0
    r = len(array)
    while l < r:
        m = (l+r) // 2
        if array[m] > target:
            r = m - 1
        elif array[m] < target:
            l = m + 1
    if array[l] > target:
        l = l - 1
    return l

height_L = [1]
N = 10**9
cache_sum1n = {}

def sum1n_(n):
    if not n in cache_sum1n:
        cache_sum1n[n] = n*(n+1)//2
    return cache_sum1n[n]
if N < 10**100:
    sum1n = lambda n: n*(n+1)//2
else:
    sum1n = sum1n_

L = 0
while height_L[-1] <= N:
    L = L + 1
    prev_height = height_L[-1]
    height_L.append(7 * prev_height)

traverse_order = []

n_remaining_rows = N

while n_remaining_rows > 7:
    next_L = 0
    next_L = binary_search(height_L, n_remaining_rows)
    traverse_order.append(next_L)
    n_remaining_rows = n_remaining_rows - height_L[next_L]

max_L = traverse_order[0] + 1

class Level:
    def __init__(self, n, prev_level, first_O, construct = True):
        self.n = n
        self.L = None
        self.n_zeros = 0
        if construct:
            self.construct_object(prev_level, first_O)
    def construct_object(self, prev_level, first_O):
        self.L = []
        for i in range(7):
            self.L.append([prev_level])
            self.n_zeros = self.n_zeros + prev_level.get_n_zeros()
            for j in range(i):
                self.L[-1].append(first_O[self.n])
                self.n_zeros = self.n_zeros + sum1n(first_O[self.n])
                self.L[-1].append(prev_level)
                self.n_zeros = self.n_zeros + prev_level.get_n_zeros()
    def get_L(self):
        return self.L
    def get_n_zeros(self):
        return self.n_zeros

L = [None]
first_O = [0, 0]
L1 = Level(1, None, first_O, construct = False)
L.append(L1)

for i in range(2, max_L+1):
    first_O.append(height_L[i-1] - 1)

for i in range(2, max_L+1):
    L.append(Level(i, L[-1], first_O))

level = L[traverse_order[0] + 1]

prev_l = 0
L_multiplier = [1]
curr_height = 0
block_row = -1
prev_block_rows = None
n_zeros = 0

for x, l in enumerate(traverse_order + [0]):
    if x > 0 and not prev_l == l:
        prev_block_rows = level.get_L()[block_row+1]
        i = 0
        for block in prev_block_rows:
            if isinstance(block, Level):
                i = i + 1
        L_multiplier.append(L_multiplier[-1] * i)
        n_zeros = n_zeros + (L_multiplier[-2] * (block_row+1)) * (sum1n(first_O[prev_l+1]) - sum1n(first_O[prev_l+1] - (N - curr_height))) # sum from 'first_O' to 'first_O - (N - curr_height)'
        block_row = 0
        level = L[l+1]
    else:
        block_row = block_row + 1
    if l == 0:
        break
    curr_height = curr_height + height_L[l]
    for block in level.get_L()[block_row]:
        if isinstance(block, int):
            n_zeros = n_zeros + L_multiplier[-1]*sum1n(block)
        else:
            n_zeros = n_zeros + L_multiplier[-1]*block.get_n_zeros()
    prev_l = l

print(sum1n(N) - n_zeros)
