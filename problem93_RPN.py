# Question: https://projecteuler.net/problem=93

# Use RPN to avoid parentheses placement

import itertools

def op(e, x, y):
    if e == '+':
        return x + y
    if e == '-':
        return x - y
    if e == '*':
        return x * y
    if e == '/':
        if not y == 0:
            return x / y
        return None

def eval(seq):
    stack = []
    for e in seq:
        if isinstance(e, int):
            stack.append(e)
        else:
            if len(stack) < 2:
                return None
            stack[-2] = op(e, stack[-1], stack[-2])
            del stack[-1]
            if not stack[-1]:
                return None
    return stack[-1]

best_seq_length = 0
permutation = None
for leaf_set in itertools.combinations([1,2,3,4,5,6,7,8,9], 4):
    result_set = set()
    for ops in itertools.product(['+', '-', '*', '/'], repeat = 3):
        for seq in itertools.permutations((ops + leaf_set)):
            result_set.add(eval(seq))
    i = 1
    while True:
        if not i in result_set:
            break
        i = i + 1
    if i-1 > best_seq_length:
        best_seq_length = i-1
        permutation = list(leaf_set)
print(best_seq_length)
print(permutation)


