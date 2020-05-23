# Question: https://projecteuler.net/problem=9

import math

# 2 * a^2 = a^2 + a^2 < a^2 + b^2 = c^2
# a < c / sqrt(2)
for a in range(1, int(1000/math.sqrt(2)) + 1):
    for b in range(a+1, 1000 - a):
        c = 1000 - a - b
        if c < b:
            break
        if c * c == a * a + b * b:
            print(a * b * c)
            exit()


