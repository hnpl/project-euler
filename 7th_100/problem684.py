# Question: https://projecteuler.net/problem=684

"""
    . Place values:
        place value 1: ones     -> (r*(10**0))
        place value 2: tens     -> (r*(10**1))
        place value 3: hundreds -> (r*(10**2))
        ...

    . Partial sum of power series:
                                   r*(n+1)-1
        r**0 + r**1 + ... + r**n = ---------
                                      r-1

    . Let s(n) be the smallest number that has a digit sum of n. Constructing s(n), assuming N = 29,
                 ________ the largest place value of tails
                | _______ u3
                || ______ u2
                ||| _____ u1
                ||||
                vvvv
        n = 1:     1
        n = 2:     2
        ...
        n = 9:     9
        n = 10:   19
        n = 11:   29
        ...
        n = 18:   99
        n = 19:  199
        n = 20:  299
        ...
        n = 27:  999
        n = 28: 1999
        n = 29: 2999
        ...
        -> Corollary: s(n) has floor(n/9) digits.

    . Let S(N) be the sum of all s(k) for all k in [1, N].
        . Let u(k) be the sum of place value k of all s(n) for all n in [1, N].
        . Let "tail" be s(n) where floor(N/9)*9 < n <= N.
            E.g. if N = 29, then the tails are 1999 and 2999.
        . Let T = N%9 -> T is the number of tails.
        . Let kappa = floor((N-T)/9) -> kappa is (the number of digits of the largest s(n) that is not tail)
        . Then S(N) = u(1) + u(2) + ... + u(kappa) + sum(tail).
        . If 1 <= k <= kappa, then
            u(k) 
            
            = [(1+2+...+9) + (9+9+...+9) * (kappa - k - 1) + 9 * T] * (10**(k-1))

            = [45 + 81 * (kappa - k - 1) + 9 * T] * (10**(k-1))

    . Find sum of u(k).
        . alpha = sum of all (45 * (10**(k-1)))                   = 5 * (10**kappa - 1)
        . beta  = sum of all (81 * (kappa - k - 1) * (10**(k-1))) = -9 * kappa + 10**kappa - 1
        . gamma = sum of all (9*T * (10**(k-1)))                  = T * (10**kappa - 1)
        . delta = sum of all largest place value of tails         = T * (T + 1) // 2 * (10**kappa)
        . For debugging:
            +-------+----------------------+---------+-----------+------------------------+
            | alpha |         beta         |  gamma  |   delta   | place value multiplier |
            +-------+----------------------+---------+-----------+------------------------+
      u1 -> |    45 |         81*(kappa-1) |     9*T |         0 |                  10**0 |
      u2 -> |    45 |         81*(kappa-2) |     9*T |         0 |                  10**1 |
      ...   |   ... |                  ... |     ... |       ... |                    ... |
      uk -> |    45 |                   81 |     9*T |         0 |          10**(kappa-1) |
  tail_1 -> |     0 |                    0 |       0 |         1 |              10**kappa |
  tail_2 -> |     0 |                    0 |       0 |         2 |              10**kappa |
      ...   |   ... |                  ... |     ... |       ... |                    ... |
  tail_T -> |     0 |                    0 |       0 |         T |              10**kappa |
            +-------+----------------------+---------+-----------+------------------------+

"""

from sage.all import *

N = 90
MOD = 10**9+7

def s(n):
    ans = 0
    base = 1
    while n > 0:
        next_digit = min(n, 9)
        ans += next_digit * base
        base *= 10
        n -= next_digit
    return ans

def S_brute_force(n):
    ans = 0
    for i in range(1, n+1):
        ans += s(i)
    return ans

def S(n):
    ans = 0

    T = n%9    # T -> Tail
    K = (n - T) // 9

    R = IntegerModRing(MOD)
    R10 = R(10)
    R10K = R10**K

    alpha = 5 * (R10K - 1)
    beta = -9 * K + R10K - 1
    gamma = T * (R10K - 1)
    delta = T * (T + 1) // 2 * (R10K)

    ans = alpha + beta + gamma + delta

    return ans

def fib_array(n):
    arr = [0, 1]
    for i in range(2, n+1):
        arr.append(arr[-1] + arr[-2])
    return arr

if __name__ == "__main__":
    ans = 0
    f = fib_array(N)
    for k in range(2, N+1):
        ans += S(f[k])
    print(lift(ans))
