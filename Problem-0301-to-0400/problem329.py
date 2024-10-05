# Problem: https://projecteuler.net/problem=329

"""
    . Use Markov process to calculate the probability of the frog to cloak the mentioned sequence.
    . Let T_P be the transition matrix where,
        T_P[i][j] is the probability of the frog to cloak "P" at position i before jumping to position j.
    . Let T_N be the transition matrix where, 
        T_N[i][j] is the probability of the frog to cloak "N" at position i before jumping to position j.
    . So,
        Case 1: i is prime
            T_P[i][j] = 2/3 * n_neighbors(i)
            T_N[i][j] = 1/3 * n_neighbors(i)
        Case 2: i is not a prime
            T_P[i][j] = 1/3 * n_neighbors(i)
            T_N[i][j] = 2/3 * n_neighbors(i)
    . So, the transition matrix of the sequence "PPPPNNPPPNPPNPN" is:
        T_seq = product(i \in "PPPPNNPPPNPPNPN") T_{i}
    . Consider starting at an integer K,
        Let S[i] be the probability of the frog to be at position i.
        So, at iteration 0, S[K] = 1 and S[i] = 0 for i != K.
        After cloaking the sequence "PPPPNNPPPNPPNPN", the probability distribution of the position of the frog is:
            S' = S * T_seq
        So, the probability of the frog to cloaking the sequence "PPPPNNPPPNPPNPN" after starting at position K is,
            Pr("PPPPNNPPPNPPNPN") = sum(S')
    . Since the starting position is uniformly distributed,
        answer = 1/500 * sum_{i \in [1, 500]} (sum(S' of starting at i))
"""

from sage.all import *

if __name__ == "__main__":
    ans = 0

    N = 500
    seq = "PPPPNNPPPNPPNPN"

    T_P = matrix(QQ, N+1, N+1)
    T_N = matrix(QQ, N+1, N+1)
    T = {"P": T_P, "N": T_N}

    for i in range(1, N+1):
        neighbors = []
        if i-1 > 0:
            neighbors.append(i-1) # left neighbor
        if i+1 <= N:
            neighbors.append(i+1) # right neighbor
        n_neighbors = len(neighbors)
        for neighbor in neighbors:
            if is_prime(i):
                T_P[i,neighbor] = Rational("{}/{}".format(2, 3)) * Rational("{}/{}".format(1, n_neighbors))
                T_N[i,neighbor] = Rational("{}/{}".format(1, 3)) * Rational("{}/{}".format(1, n_neighbors))
            else:
                T_P[i,neighbor] = Rational("{}/{}".format(1, 3)) * Rational("{}/{}".format(1, n_neighbors))
                T_N[i,neighbor] = Rational("{}/{}".format(2, 3)) * Rational("{}/{}".format(1, n_neighbors))

    Tseq = copy(T[seq[0]])
    for letter in seq[1:]:
        Tseq = Tseq * T[letter]

#    for i in range(1, N+1):
#        S = matrix(QQ, 1, N+1)
#        S[0, i] = Rational("1/1")
#        S = S * Tseq
#        ans = ans + Rational("1/500") * sum(S[0])


    S = matrix(QQ, 1, N+1)
    for i in range(1, N+1):
        S[0, i] = Rational("1/500")
    S = S * Tseq
    ans = sum(S[0])
    print(ans)


