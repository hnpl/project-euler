# Question: https://projecteuler.net/problem=301

import numpy as np

# According to this https://en.wikipedia.org/wiki/Nim, the next player loses when the XOR of 3 heaps is 0.
# When XOR(n, 2n, 3n) == 0? It occurs when n has consecutive 1's.
# Why?
#           bit    b1 b2 b3
# carry of n+2n     x  y  z   
#             n = ..a  1  1...
#            2n = ..1  1  b...
#            3n = ..c  d  e...
#  Case 1. z = 0
#    Case 1.1. a = 0, b = 0
#       -> cde = 001
#       -> bit b1 of XOR(n, 2n, 3n) = 1 -> XOR != 0
#    Case 1.2. a = 0, b = 1
#       -> cde = 010
#       -> bit b2 of XOR(n, 2n, 3n) = 1 -> XOR != 0
#    Case 1.3. a = 1, b = 0
#       -> cde = 101
#       -> bit b1 of XOR(n, 2n, 3n) = 1 -> XOR != 0
#    Case 1.4. a = 1, b = 1
#       -> cde = 110
#       -> bit b2 of XOR(n, 2n, 3n) = 1 -> XOR != 0
#  Case 2. z = 1
#    Case 2.1. b = 0
#       -> d = 1
#       -> bit b2 of XOR(n, 2n, 3n) = 1 -> XOR != 0
#    Case 2.2. b=1
#       -> e = 1
#       -> bit b3 of XOR(n, 2n, 3n) = 1 -> XOR != 0

N = 30 # 2^30 is 1000...000 with 30 zeros -> 31 digits in base 2 | we only need to count the case of <=30 digits and manually add the case of 2^30


# DP[i][j] -> number of ways of generate a base 2 number of <=i-digit that ends with j
DP = np.zeros((N+1, 2), dtype = np.uint32)
DP[1][0] = 1
DP[1][1] = 1

for i in range(2, N+1):
    DP[i][0] = DP[i-1][0] + DP[i-1][1]
    DP[i][1] = DP[i-1][0]

print(DP[N][0] + DP[N][1] + 1 - 1)
#                         ^^^^^^^
#                         add the case of 2^30, remove the case of 0
