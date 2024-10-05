# Question: https://projecteuler.net/problem=377

from sage.all import *

"""
- Strategy:
    - Let f(n) = sum of numbers with digit sum of n.
    - Let g(n) = number of numbers with digit sum of n.
    - Suppose we add a digit 3 to all numbers with digit sum of n-3,
        * For each number K having digit sum of n-3,
            - The new number is K*10+3, and the digit sum of the new number is n.
        * So, the sum of all K with digit 3 appended is,
            new_sum = sum(K*10+3, for all K)
                    = sum(K*10, for all K) + sum(3, for all K)
                    = 10 * sum_of_K + 3 * number_of_such_K
                    = 10*f(n-3) + 3*g(n-3)
    - We can generalize the new_sum function above to any digit j,
            new_sum_appending_digit_d = 10*f(n-j) + j*g(n-j)
        * So, we can express f(n) by appending 1 to numbers in f(n-1), 2 to numbers in f(n-2), etc
        * In other words,
            f(n+1) = sum(10*f(n-j) + j*g(n-j), for j in [0..8])
        * Obviously,
            g(n+1) = sum(g(n-j) for j in [0..8]) = 2**(n)
    - Those are linear relations, so we can use matrix modular exponentiation to find f(n) for big values of n.
    - We set up S as a 18x1 matrix such that,
        S[j] = g(n-j) for all j in [0..8]
        S[k] = f(n-k) for all k in [0..8]
        meaning,
        S = [g(n),
             g(n-1),
             ...,
             g(n-8),
             f(n),
             f(n-1),
             ...,
             f(n-8)]
    - The transformation matrix T corresponding to the layout of S should be,
        [ 1  1  1  1  1  1  1  1  1  0  0  0  0  0  0  0  0  0]
        [ 1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  0  0]
        [ 0  0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  0  0]
        [ 1  2  3  4  5  6  7  8  9 10 10 10 10 10 10 10 10 10]
        [ 0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  0  1  0  0  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0  0]
        [ 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1  0]

    - We set up the initial S such that g(0) = 1.
"""

N = 17
MOD = 10**9

f_n_offset = 9
g_n_offset = 0

if __name__ == "__main__":
    R = IntegerModRing(MOD)

    T = matrix(R, ncols=18, nrows=18)
    for j in range(8):
        T[g_n_offset + j + 1, g_n_offset + j] = 1 # g(n-j) becomes g(n-j-1) in the next iteration
    for j in range(9):
        T[g_n_offset, g_n_offset + j] = 1 # g(n) = sum(g(n-j))
    for j in range(8):
        T[f_n_offset + j + 1, f_n_offset + j] = 1 # f(n-j) becomes f(n-j-1) in the next iteration
    for j in range(9):
        T[f_n_offset, f_n_offset + j] = 10 # f(n) = sum(j*g(n-j) + 10*f(n-j))
    for j in range(9):
        T[f_n_offset, g_n_offset + j] = j+1
    print(T)

    S = matrix(R, ncols=1, nrows=18)
    S[g_n_offset, 0] = 1

    print(S)

    #print((T**5)*S)
    ans = 0
    prev = 0
    curr = 0
    for i in range(1, N+1):
        curr = 13**i
        step = curr-prev
        S = (T**step) * S
        ans += S[f_n_offset, 0]
        prev = curr
    print(lift(ans))
