# Problem: https://projecteuler.net/problem=323

import numpy as np

"""
    T: transistion matrix where,
        T[i][j] is the probability of (a state where i bits are set) transition to (a state where j bits are set).
        We have:
            T[i][j] = C(N-i, j-i) * 2^i / 2^N
        Why?
            Assume S' = S | y, where S has i bits set, while S' has j bits set.
            For a state S where i bits are set, there are (N-i) are not set.
            To transition to S' where 2^j bits are set, we need to pick (j-i) bits from (N-i) bits.
            There are C(N-i, j-i) ways to pick (j-i) bits from (N-i) bits.
            Also, y might have bits that are already set in S, and there are 2^i ways to choose those bits.
            Therefore, there are C(N-i, j-i) * 2^i ways to choose y.
            Also, there are 2^N different N-bit numbers.
            Therefore, T[i][j] = C(N-i, j-i) * 2^i / 2^N.
    S: state vector where,
        S[i] means the probability of the current number has i bits set.

    Apply Markov chain, we can figure out the probability of having 2^N bits set after applying bitwise-OR k times as follows,
        S_{k} = S * T^k
    To calculate the expected value of N, and due to linearity of expectations, we can follow the following formula to calculate E(N):
        E(N) = sum_{i = 1 to inf} (S[N] - prev_S[N]) * i
    We expect this series to converge, so we calculate the sum for each K as in sum_{i = 1 to K} (S[N] - prev_S[N]) * i until it converges.
"""

N = 32

factorials = [1]
for i in range(1, N+1):
    factorials.append(factorials[-1] * i)

def C(n, k):
    return factorials[n] / factorials[k] / factorials[n-k]

T = np.zeros((N+1, N+1), dtype = np.float)

for alpha in range(N+1):
    for beta in range(alpha, N+1):
        T[alpha][beta] = 1.0 * C(N - alpha, beta - alpha) / (2**(N - alpha))

S = np.zeros((N+1), dtype = np.float)
S[0] = 1.0

ans = 0
prev_ans = -1
stop_threshold = 10
stop_counter = 0
tol = 10**-12

prev_SN = 0.0
n_trials = 0
while True:
    n_trials = n_trials + 1
    S = S.dot(T)
    ans = ans + (S[N]-prev_SN) * n_trials
    prev_SN = S[N]
    if abs(ans - prev_ans) < tol:
        stop_counter = stop_counter + 1
        if stop_counter == stop_threshold:
            break
    else:
        stop_counter = 0
    prev_ans = ans

print(ans)
