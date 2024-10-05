# Question: https://projecteuler.net/problem=158
# General idea:
#     - Pick an decreasing sequence S, e.g. (n,n-1,...,1). There are C(26, n) ways of choosing S.
#     - Assume the sequence we're going to construct consists of the left seq and the right seq, which is a partition of S.
#     - Let the "pivot" be the character that comes lexicographically after its neighbour on the left, i.e. the first character of the right seq.
#     - For each possible length k of the left seq,
#         * There are C(n, k) ways of choosing the left seq.
#         * However, among those C(n,k) seqs, there is always one seq such that all characters on the right seq are lexicographically after all characters on the left. In other words, left = (n, n-1, ..., n-k+1), right = (n-k, n-k-1, ..., 1).
#         * For each of the C(n,k)-1 seqs on the left, there is always only one possible of choosing the right seq. This is because the right seq must be a decreasing seq.
#     - So, p(n) = C(26, n) * sum_{k=1}^(k=n}[C(n, k) - 1]
#     - We have that sum_{k=0}^{k=n}[C(n,k)]   = 2^n,
#                 so sum_{k=1}^{k=n}[C(n,k)]   = 2^n - 1,
#                 so sum_{k=1)^{k=n}[C(n,k)-1] = 2^n - 1 - n
#     - So, p(n) = C(26, n) * (2^n - n - 1)

from scipy.special import comb # comb(n,k) = n choose k

M = 26 # number of characters in the alphabet

def C(n,k):
    return comb(n, k, exact = True)

def sum_from_a_to_b(a, b):
    return (b-a+1)*(a+b)//2

def p(n):
    return C(26, n) * (2**n - n - 1)
if __name__ == "__main__":
    ans = 0

    for n in range(1, M+1):
        p_n = p(n)
        ans = max(ans, p_n)

    print(ans)
