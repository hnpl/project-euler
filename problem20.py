# Question: https://projecteuler.net/problem=20

import math

N = 100

num = math.factorial(N)

print(sum(map(int, list(str(num)))))
