# Question: https://projecteuler.net/problem=34

import math

fac9 = math.factorial(9)

N = 1

while fac9 * N >= 10**(N-1):
    N = N + 1

s = 0

factorial = {}
for i in range(10):
    factorial[i] = math.factorial(i)

for i in range(1, fac9 * N):
    digits = map(int, list(str(i)))
    fac_sum = sum(map(lambda x: factorial[x], digits))
    if fac_sum == i:
        s = s + i

# The problem statement excludes 1! = 1 and 2! = 2 from the result
print(s - 1 - 2)
