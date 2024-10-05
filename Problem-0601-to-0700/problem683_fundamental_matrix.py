# Problem: https://projecteuler.net/problem=683

"""
. Same idea as problem 227.
. Use the distance between the dice as a state.
. Use Markov chain to track the probabilities.

. E[(X_2)^2 +(X_3)^2 + ... + (X_n)^2] = E[(X_2)^2] + E[(X_3)^2] + ... + E[(X_n)^2] (linearity of expectation)

. E(X^2) = [E(X)]^2 + Var(X)

. Use the formula here to find Var(X) and E(X) from the fundamental matrix,
    https://en.wikipedia.org/wiki/Absorbing_Markov_chain#Expected_number_of_steps
"""

import numpy as np

N = 500

def E_X2(N):
    ans = 0.0

    T = np.zeros((N, N), dtype = np.double)


    T[0][0] = 0
    for delta in range(1, N):
        # player 1 rolls [1,2], player 2 rolls [1,2]: delta doesn't change
        # player 1 rolls [1,2], player 2 rolls [3,4]
        T[delta][(delta-1)%N] += 4/36
        # player 1 rolls [1,3], player 2 rolls [5,6]
        T[delta][(delta-2)%N] += 4/36

        # player 1 rolls [3,4], player 2 rolls [1,2]
        T[delta][(delta+1)%N] += 4/36

        # player 1 rolls [3,4], player 2 rolls [5,6]
        T[delta][(delta-1)%N] += 4/36

        # player 1 rolls [5,6], player 2 rolls [1,2]
        T[delta][(delta+2)%N] += 4/36
        # player 1 rolls [5,6], player 2 rolls [3,4]
        T[delta][(delta+1)%N] += 4/36
        # player 1 rolls [5,6], player 2 rolls [5,6]: delta doesn't change

        T[delta][delta] += 1.0 - np.sum(T[delta])

    fundamental_matrix = np.linalg.inv(np.eye(N) - T)

    #t = fundamental_matrix.dot(np.ones((N,1)))
    t = np.sum(fundamental_matrix, axis = 1)
    E_steps_per_state = t-1
    E2_X = E_steps_per_state*E_steps_per_state

    V_steps_per_starting_state = (2*fundamental_matrix - np.eye(N)).dot(t) - t * t
    V_X = V_steps_per_starting_state
    
    ans = np.sum(E2_X + V_X) / N

    return ans


def G(N):
    ans = 0.0
    for i in range(2, N+1):
        ans += E_X2(i)
    return ans

if __name__ == "__main__":
    print(G(N))