# Problem: https://projecteuler.net/problem=258

"""
    Calculating the lagged Fibonacci sequence via matrix multiplication.
"""

from sage.all import *
import numpy as np

N = 10**18

M = 20092010
MOD1 = 2
MOD2 = 5
MOD3 = 859
MOD4 = 2339
assert(M == MOD1 * MOD2 * MOD3 * MOD4)

K = 2000

if __name__ == "__main__":
    ans = 0

    T = np.zeros((K, K), dtype = np.int)
    idx = np.arange(K)
    T[idx, idx] = 1
    idx = np.arange(K-1)
    T[idx, idx+1] = 1
    # g_(1999) = g_(-1) + g_(0) # negative indicates value in the previous iteration
    #                     ^^^^^
    #                   don't have this in the previous iteration
    #                   but, g_(0) = g_(-1999) + g(-2000)
    T[K-1, 0] = 1
    T[K-1, 1] = 1

    S = np.ones((K, 1), dtype = np.int)

    residues = {}

    for mod in [MOD1, MOD2, MOD3, MOD4]:
        R = IntegerModRing(mod)

        TR = matrix(R, T) 

        SR = matrix(R, S)

        SR = np.dot(TR**(N//K), SR)

        residues[mod] = lift(SR[0, 0])

    m = 1
    r = 0
    for mod in [MOD1, MOD2, MOD3, MOD4]:
        r = crt(r, residues[mod], m, mod)
        m = m * mod

    ans = r

    print(ans)
