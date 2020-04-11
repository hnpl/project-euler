# Question: https://projecteuler.net/problem=28

# This problem totally reminds me of what I did in 2009 ...
# TODO: explain why this sum is a third degree polynomial ;)

N = 1001

n = (N + 1) // 2

print(16 * (2*n*n*n + 3*n*n + n) // 6 - 14 * (n*n + n) + 16*n - 3)
