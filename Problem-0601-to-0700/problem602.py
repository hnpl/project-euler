# Question: https://projecteuler.net/problem=602

"""
  . Let f(n) be the expected score of each friend after n turns,
    f(0) = 0
    f(1) = C(1,1) * 1 * (1-p)
    f(2) = C(2,1) * 1 * (1-p) * p   + C(2,2) * 2 * (1-p)^2
    f(3) = C(3,1) * 1 * (1-p) * p^2 + C(3,2) * 2 * (1-p)^2 * p + C(3,3) * 3 * (1-p)^3
    ...
    f(n) = sum_{j=1 to n} (j * C(n, j) * (1-p)^j * p^{n-j})
        where j is the score (or number of heads)
              C(n,j) is the number of ways of choosing turns to be head
              (1-p)^j is the probability of turns that have heads
              p^(n-j) is the probability of turns that have tails
  . Also note that, j * C(n,j) = n * C(n-1, j-1), so,
    f(n) = sum_{j=1 to n} (n * C(n-1, j-1) * (1-p)^j * p^{n-j})
         = n * (1-p) * sum_{j=0 to n-1} (C(n-1, j) * (1-p)^j * p^{n-1-j})
         = n * (1-p) * (1-p+p)^n
         = n * (1-p)
  . So, since the events of getting H's or T's are independent among the friends, E(X_1*X_2*X_3*...*X_n) = E(X_1) * E(X_2) * E(X_3) * ... * E(X_n).
  . So, let g(n) be the expected product after n turns, then
    g(n) = f(n)^n = n^n * (1-p)^n
  . Let h(n) be the expected product,
    h(n) =   p * 0
           + p(1-p) * (f(1))^n
           + p^2*(1-p) * (f(2))^n
           + p^3*(1-p) * (f(3))^n
           + ...
         =   1^n * p   * (1-p)^{n+1)
           + 2^n * p^2 * (1-p)^{n+1}
           + 3^n * p^3 * (1-p)^{n+1}
           + ...
  . So, c(n,k) = sum_{j = 1 to k} (1^(k-j) * j^n * C(n+1, k-j))
"""


from sage.all import *

N = 10000000
K = 4000000
MOD = 10**9+7

def pow_mod(base, exp, mod):
    ans = 1

    while exp > 0:
        if exp % 2 == 1:
            ans *= base
            ans %= mod
        base *= base
        base %= mod
        exp //= 2

    return ans

def solve(n, k, mod):
    ans = 0

    precomputed_C_n_j = [0] * k
    precomputed_C_n_j[0] = 1
    for j in range(1, k):
        if j % 100000 == 0:
            print(j)
        precomputed_C_n_j[j] = precomputed_C_n_j[j-1] * (n+2-j) * inverse_mod(j, mod)
        precomputed_C_n_j[j] %= mod
    for j in range(1, k+1):
        #print(f"(-1)^{k-j} . ({j})^{n} . p . C({n+1}, {n-j}) . p^{k-j} = {(-1)**((k-j)%2) * pow_mod(j, n, mod) * precomputed_C_n_j[k-j]}")
        ans += (-1)**((k-j)%2) * pow_mod(j, n, mod) * precomputed_C_n_j[k-j]
        ans %= mod

    return ans

if __name__ == "__main__":
    ans = solve(N, K, MOD)
    print(ans)
