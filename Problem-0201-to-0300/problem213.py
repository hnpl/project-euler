# Problem: https://projecteuler.net/problem=213

"""
    . Let N = 30

    . Let squares = { s_0, s_1, ..., s_899 } be the set of all squares in the grid,
        where s_i indicates the square at (i//N, i%N).
    
    . Let fleas = { f_0, f_1, ..., f_899 } be the set of all fleas
        where f_0 indicates the flea that is at position (i//N, i%N) at the beginning.

    . To simplify the notation, we let "index idx" indicate "the position (idx // N, idx % N)"

    . Due to the linearity of expectation, the expected number of empty squares is the sum of expected number of empty squares of EACH square; i.e.,
        E(N_empty) = sum_{s \in squares} {1 * P(s is empty) + 0 * [1 - P(s is empty)}
                   = sum_{s \in squares} P(s is empty)

    . To determine the probability of square s being empty,
        P(s is empty) = product_{t \in fleas} P(flea t is not in s after 50 iterations)

    . We use Markov process to determine the probability of each flea being at each square after 50 iterations. For each flea k:
        . Let T be the transistion matrix where
            T[i][j] = the probability of a flea at index i jumps to index j.
            so, T[i][j] = 1 / n_neighbors
        . Let S be the probability matrix where S[i] indicate the probability of flea k being at index i.
            So, at iteration 0, S[i] = 1 for i = k, and S[i] = 0 otherwise.
        . Let S_50 be the probability matrix where S[i] indicate the probability of flea k being at index i after 50 iterations.
            So, S_50 = S * matrix_power(T, 50)

    . So, P(s is empty) = (1 - S_50[s] of flea 0) * (1 - S_50[s] of flea 1) * ... * (1 - S_50[s] of flea 899)
"""

import numpy as np

N = 30
M = 50

def index_to_position(idx):
    return idx // N, idx % N

def position_to_index(x, y):
    return x * N + y

def get_neighbors(idx):
    x, y = index_to_position(idx)

    available_positions_idx = []
    for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
        fx, fy = x + dx, y + dy
        if fx >= 0 and fx < N and fy >= 0 and fy < N:
            available_positions_idx.append(position_to_index(fx, fy))

    return available_positions_idx

if __name__ == "__main__":

    ans = 0

    T = np.zeros((N*N, N*N), dtype = np.float)
    for idx in range(N*N):
        neighbors_idx = get_neighbors(idx)
        n_neighbors_idx = len(neighbors_idx)
        for f_idx in neighbors_idx:
            T[idx][f_idx] = 1 / n_neighbors_idx 

    TpM = np.linalg.matrix_power(T, M)

    P_empty = np.ones(N*N, dtype = np.float)

    S = np.zeros((1, N*N), dtype = np.float)

    for flea_idx in range(N*N):
        S = np.zeros((1, N*N), dtype = np.float)
        S[0][flea_idx] = 1.0
        S = S.dot(TpM)
        P_empty = np.multiply(P_empty, 1.0 - S)

    ans = np.sum(P_empty)

    print(ans)
