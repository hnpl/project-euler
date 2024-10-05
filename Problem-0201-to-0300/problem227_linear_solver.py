# Problem: https://projecteuler.net/problem=227

"""
. Define distance as the clockwise number of people between the dice.
. Use the distance between the dices as a state.
. Use Markov chain to track the probabilities.
. T[distance1][distance2] is the probability of transitioning from distance 1 to distance 2

Solve for the expected number of steps from T:
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
"""

import numpy as np

N = 100

if __name__ == "__main__":
    ans = 0.0

    T = np.zeros((N, N), dtype = np.double)
    S = np.zeros((N), dtype = np.double)

    S[50] = 1.0

    #T[0][0] = 1.0

    for delta in range(1, N):
        # player 1 rolls 1, player 2 rolls 1: delta doesn't change
        # player 1 rolls 1, player 2 rolls [2..5]
        T[delta][(delta-1)%N] += 4/36
        # player 1 rolls 1, player 2 rolls 6
        T[delta][(delta-2)%N] += 1/36

        # player 1 rolls [2..5], player 2 rolls 1
        T[delta][(delta+1)%N] += 4/36

        # player 1 rolls [2..5], player 2 rolls 6
        T[delta][(delta-1)%N] += 4/36

        # player 1 rolls 6, player 2 rolls 1
        T[delta][(delta+2)%N] += 1/36
        # player 1 rolls 6, player 2 rolls [2..5]
        T[delta][(delta+1)%N] += 4/36
        # player 1 rolls 6, player 2 rolls 6: delta doesn't change

        T[delta][delta] = 18/36

    y = np.ones((N, 1), dtype = np.double)
    y[0,0] = 0

    x = np.linalg.solve(np.eye(N) - T, y)
    print(x[50][0])
