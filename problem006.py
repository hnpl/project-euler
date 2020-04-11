# Question: https://projecteuler.net/problem=6

# sum i^2 = N(N+1)(2N+1)/6
# (sum i)^2 = N^2 * (N+1)^2 / 4

N = 100
print(N * N * (N+1) * (N+1) // 4 - N*(N+1)*(2*N+1) // 6)