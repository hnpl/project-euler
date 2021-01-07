# Question: https://projecteuler.net/problem=183

"""
    . d/dk (N/k)^k = (N/k)^k * (log(N/k) - 1)
    . The value of (N/k)^k is maximized when the derivative is 0, i.e. log(N/k) - 1 = 0
                                                                  so, N / k = e
                                                                  so, k = N / e
    . M(N) = (p / q)**q has a finite number of decimal places when:
        * we have that p' / q' = p / q; gcd(p', q') = 1; and 2 and 5 are the only prime factor of q.
"""

from math import e, gcd

N = 10000

def argmax(L):
    max_pos = 0
    max_val = L[max_pos]
    for pos, val in enumerate(L):
        if max_val < val:
            max_pos = pos
            max_val = val
    return max_pos

def D(N):
    k = N / e
    k = round(k)
    #k = int(k)
    #candidates = [k-2, k-1, k, k+1, k+2]
    #P = [(N/k)**k if k != 0 else 0 for k in candidates]
    #k = candidates[argmax(P)] # the value of k such that (N/k)^k is maximized
    
    # reduce the fraction of N/k
    reduced_k = k // gcd(N, k)
    # check if k is irrational
    while reduced_k % 2 == 0:
        reduced_k //= 2
    while reduced_k % 5 == 0:
        reduced_k //= 5
    if reduced_k == 1:
        return -N
    return N

if __name__ == "__main__":
    ans = 0
    for i in range(5, N+1):
        ans += D(i)
    print(ans)
