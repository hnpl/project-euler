# Question: https://projecteuler.net/problem=624

from sage.all import *

def Fib(N, MOD):
    R = IntegerModRing(MOD)
    A = matrix(R, [[1, 1], [1, 0]])
    B = matrix(R, [[1],[0]])
    C = (A ** (N-1)) * B
    return C[0][0]

def Alpha(N, MOD):
    R = IntegerModRing(MOD)
    A = matrix(R, [[1, 1], [1, 0]])
    B = matrix(R, [[3],[1]])
    C = (A ** (N-1)) * B
    return C[1][0]

def Q(N, MOD):
    R = IntegerModRing(MOD)
    two_to_N = R(2) ** N
    rN = two_to_N * Fib(N-1, MOD) - (-1)**N
    rD = two_to_N * two_to_N - two_to_N * Alpha(N, MOD) + (-1)**N
    return rN/rD

print(Q(10**18, 10**9 + 9))