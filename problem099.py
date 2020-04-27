# Quesiton: https://projecteuler.net/problem=99

# Convert x^y to 10^(alpha), then we compare alpha
# x^y = 10^alpha
# y * log_10(x) = alpha

import math

# 0-indexed
max_line = -1
max_alpha = -1

with open("inputs/p099_base_exp.txt", "r") as f:
    for i, line in enumerate(f):
        line = line.strip().split(',')
        x = int(line[0])
        y = int(line[1])
        alpha = y * math.log10(x)
        if alpha > max_alpha:
            max_line = i
            max_alpha = alpha

print(max_line + 1)
