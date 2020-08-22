# Problem: https://projecteuler.net/problem=227

# Define distance as the clockwise number of people between the dice.
# Use the distance between the dices as a state.
# Use Markov chain to track the probabilities.
# T[distance1][distance2] is the probability of transitioning from distance 1 to distance 2

import numpy as np

N = 100

def position_to_index(x, y):
    return x * N + y

def index_to_position(idx):
    return idx // N, idx % N

def new_position(current_position, roll):
    if roll == 1:
        current_position = (current_position - 1) % N
    elif roll == 6:
        current_position = (current_position + 1) % N
    return current_position

if __name__ == "__main__":
    ans = 0.0

    T = np.zeros((N, N), dtype = np.double)
    S = np.zeros((N), dtype = np.double)

    S[50] = 1.0

    T[0][0] = 1.0

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

        T[delta][delta] = 1.0-np.sum(T[delta])

    prev_probability = 0.0
    curr_probability = 0.0

    prev_expectation = -1.0
    curr_expectation = 0.0


    tol = 1e-12
    n_turns = 0

    while (((curr_expectation - prev_expectation) > tol) or (curr_probability < tol)):
        n_turns = n_turns + 1
        prev_probability = curr_probability
        prev_expectation = curr_expectation

        S = S.dot(T)

        curr_probability = S[0]

        curr_expectation = curr_expectation + n_turns * (curr_probability - prev_probability)

    ans = curr_expectation
    print("{:.6f}".format(ans))
