# Problem: https://projecteuler.net/problem=683

"""
. Same idea as problem 227_linear_solver.
. Use the distance between the dice as a state.
. Use Markov chain to track the probabilities.
. T[distance1][distance2] is the probability of transitioning from distance 1 to distance 2

. E[(X_2)^2 +(X_3)^2 + ... + (X_n)^2] = E[(X_2)^2] + E[(X_3)^2] + ... + E[(X_n)^2] (linearity of expectation)

. Solve for the expected number of steps from T:
    . Let E(X->Y) indicate the expected number of steps of going from state X to state Y.
    . We have this relatation:
        E(X->Y) = sum_{all next_state}(P(X->next_state) * [E(next_state->Y) + 1])
                = 1 + sum_{all next_state}(P(X->next_state) * E(next_state->Y))
                = P(X->X-2)  * E(X-2->Y // for this problem
                + P(X->X-1)  * E(X-1->Y)
                + P(X->X)    * E(X->Y)
                + P(X->X+1)  * E(X+1->Y)
                + P(X->X+2)  * E(X+2->Y)
                + 1
    . We have, for example,
        E(30->0) = 1/36  * [E(28->0) + 1]
                 + 8/36  * [E(29->0) + 1]
                 + 18/36 * [E(30->0) + 1]
                 + 8/36  * [E(31->0) + 1]
                 + 1/36  * [E(32->0) + 1]
    . Construct a matrix A from the above relations, i.e.:
        A * E = y
     where E[X] = E(X->0), 
           y[X] = 0 if X is the final state, y[X] = 1 otherwise
           A = (I - T)
    . Solve for E.

. Solve for the expected squared number of steps from T and E:
    . https://math.stackexchange.com/questions/1181489/how-to-compute-the-variance-of-number-of-coin-flips-to-see-hth-sequence-using-li
    . Let F(X->Y) indicate the expected number of steps of going from state X to state Y.
    . We have this relatation:
        F(X->Y) = sum_{all next_state} [(k+1)^2 * P(X->next_state, n_steps(X->next_state) = k)]
                = sum_{all next_state} [k^2 * P(X->next_state, n_steps(X->next_state) = k) + 2 * k * P(X->next_state, n_steps(X->next_state) = k) + P(X->next_state, n_steps(X->next_state) = k)]
                = sum_{all next_state} [k^2 * P(X->next_state, n_steps(X->next_state) = k)]
                  + sum_{all next_state} [2 * k * P(X->next_state, n_steps(X->next_state) = k)]
                  + sum_{all next_state} [P(X->next_state, n_steps(X->next_state) = k)]
                = sum_{all next_state} (F(X->next_state)) + 2 * k * sum_{all next_state} (E(X->next_state)) + 1
    . (I - T) * F = T * (2*E) + 1
      Solve for F.
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

        #T[delta][delta] = 1.0 - np.sum(T[delta])
        T[delta][delta] += 1.0 - np.sum(T[delta])

    A = np.eye(N) - T
    rhs = np.ones((N, 1), dtype = np.double)
    rhs[0][0] = 0
    E = np.linalg.solve(A, rhs)
    
    G = T.dot(2*E) + 1
    rhs = G
    rhs[0][0] = 0
    E2 = np.linalg.solve(A, rhs)
    ans = np.sum(E2)/N
    return ans


def G(N):
    ans = 0.0
    for i in range(2, N+1):
        ans += E_X2(i)
    return ans

if __name__ == "__main__":
    print(G(N))