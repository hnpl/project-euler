# Question: https://projecteuler.net/problem=123

# From problem 120:
#
#   n |    X_n = (a-1)^n + (a+1)^n | mod a^2
#-----|----------------------------|--------
#   1 |                         2a | 2a
#   2 |                   2a^2 + 2 | 2
#   3 |                  2a^3 + 6a | 6a
#   4 |            2a^4 + 6a^2 + 2 | 2
#   5 |         2a^5 + 20a^3 + 10a | 10a
#   6 |   2a^6 + 30a^4 + 30a^2 + 2 | 2
#   7 | 2a^7 + 42a^5 + 70a^3 + 14a | 14a
#
# So, for n that are even, r = 2.
#     for n that are odd,  r = 2 * p_n * n

N = 10**10

with open('../inputs/primes_1e6.txt', 'r') as f:
    n = 1
    prime = int(f.readline()) # 1st prime
    while True:
        prime = int(f.readline()) # 2k-th prime
        n = n + 1
        prime = int(f.readline()) # 2k+1 th prime
        n = n + 1
        if 2 * prime * n > N:
            break
print(n)