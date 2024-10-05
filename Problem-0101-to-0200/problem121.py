# Question: https://projecteuler.net/problem=121

# Turn 1: (1/2 r + 1/2 b)
# Turn 2: (1/2 r + 1/2 b) * (2/3 r + 1/3 b)
# Turn 3: (1/2 r + 1/2 b) * (2/3 r + 1/3 b) * (3/4 r + 1/4 b) 
# The probabilities are in the coefficients
# E.g., turn 3: 1/24*b^3 + 1/4*b^2*r + 11/24*b*r^2 + 1/4*r^3 -> P(3b) = 1/24;
#                                                            -> P(2b,1r) = 1/4
#                                                            -> P(1b,2r) = 11/24
#                                                            -> P(3r) = 1/4

from sage.all import *

var('r, b')

f = 1

N = 15

for i in range(N):
    f = f * ((i+1)*r/(i+2) + 1*b/(i+2))

f = expand(f)

p_win = 0
for i in range(N//2+1, N+1): # [N/2 + 1, N]
    p_win = p_win + f.coefficient(r**(N-i) * b**(i))

ans = floor(p_win ** (-1))

print(ans)
