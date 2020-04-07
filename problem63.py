# Question: https://projecteuler.net/problem=63

import math

i = 0
count = 0

# for every i, check x^i to y^i, where x^i >= 10^(i-1) and y^i < 10^i
#                                        x >= 10^((i-1)/i)
#                                                          y < 10

while True:
    i = i + 1
    x = 10 ** ((i-1)/i)
    y = 9
    if x > y:
        break
    count = count + y - math.ceil(x) + 1

print(count)
