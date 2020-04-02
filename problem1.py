# Question: https://projecteuler.net/problem=1

N = 1000

# A: number of integers in [1, N) that are divisible by 3
# B: number of integers in [1, N) that are divisible by 5
# C: number of integers in [1, N) that are divisible by 15 = 3 * 5

A = (N - 1) // 3
B = (N - 1) // 5
C = (N - 1) // 15

# sumA: sum of integers in [1, N) that are divisible by 3
# sumB: sum of integers in [1, N) that are divisible by 5
# sumC: sum of integers in [1, N) that are divisible by 15

sumA = 3 * (A * (A + 1)) // 2
sumB = 5 * (B * (B + 1)) // 2
sumC = 15 * (C * (C + 1)) // 2

print("{}".format(sumA + sumB - sumC))
