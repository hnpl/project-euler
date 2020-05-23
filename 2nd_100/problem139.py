# Question: https://projecteuler.net/problem=139

# we have: a^2 + b^2 = c^2; a - b = x; x | c (because of tiling)
# Suppose a + b + c = P
#         2a - x + k * x = P
#
# a^2 + b^2 = c^2
# a^2 + (a-x)^2 = c^2
# 2a^2 - 2ax + x^2 = k^2 x^2
# 
# a^2 - ax = (k^2 - 1)/2 * x^2 
# (a/x)^2 - (a/x) = (k^2-1)/2
# (a/x)^2 - (a/x) + 1/4 = (k^2-1)/2 + 1/4
# (a/x - 1/2)^2 -(k^2-1)/2 = 1/4
# (2a/x - 1)^2 - 2(k^2-1) = 1
# (2a/x - 1)^2 - 2k^2 = -1
# Let 2a/x - 1 = q
# q^2 - 2k^2 = -1 (*)
#
# From a + b + c = P
# 2a - x + kx = P
# qx + x - x + kx = P
# (q+k)x = P (**)
#
# Solve for q, k from (*) (Pell's equation -> there're recurrence functions that find q, k)
# https://en.wikipedia.org/wiki/Pell%27s_equation
# Put q, k to (**) to find x
# https://math.stackexchange.com/questions/1268902/solutions-of-the-pell-type-equation-x2-2y2-1 

q = 1
k = 1
q, k = 3*q + 4*k, 2*q + 3*k # this is needed, because if q = 1 and k = 1 -> 2a = x and c = x, so b = 0 (while b > 0)
N = 10**8
n_solutions = 0
while q + k <= N:
    n_solutions = n_solutions + N // (q+k)
    q, k = 3*q + 4*k, 2*q + 3*k

print(n_solutions)

