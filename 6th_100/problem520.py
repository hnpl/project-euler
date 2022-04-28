# Question: https://projecteuler.net/problem=520

import numpy as np

N = 39
MOD = 1_000_000_123

def generate_all_states():
    next_index = 0
    states = {}
    ans_states = []
    rev_map = {}
    for n_presented_odds in range(6):
        for n_presented_evens in range(6):
            for n_odd_odds in range(n_presented_odds+1):
                for n_even_evens in range(n_presented_evens+1):
                    states[(n_presented_odds, n_presented_evens, n_odd_odds, n_even_evens)] = next_index
                    rev_map[next_index] = (n_presented_odds, n_presented_evens, n_odd_odds, n_even_evens)
                    if n_presented_odds == n_odd_odds and n_presented_evens == n_even_evens:
                        ans_states.append(next_index)
                    next_index += 1

    return states, ans_states, rev_map

cached_pow = {}

def pow(M, n):
    base = np.array(M, dtype=object)
    k, _ = M.shape
    result = np.eye(k, dtype=object)
    base_exp = 1
    while n > 0:
        if n % 2 == 1:
            result = np.matmul(result, base)
            result = np.mod(result, MOD)
        n //= 2
        base_exp *= 2
        if not base_exp in cached_pow:
            base = np.matmul(base, base)
            base = np.mod(base, MOD)
            cached_pow[base_exp] = base
        base = cached_pow[base_exp]
    return result

if __name__ == "__main__":
    states, ans_states, rev_map = generate_all_states()
    n_states = len(states)
    m = n_states+1

    T = np.zeros((m, m), dtype=object)
    for n_presented_odds in range(6):
        for n_presented_evens in range(6):
            for n_odd_odds in range(n_presented_odds+1):
                for n_even_evens in range(n_presented_evens+1):
                    curr_state = states[(n_presented_odds, n_presented_evens, n_odd_odds, n_even_evens)]

                    # Adding an odd digit to numbers having an odd amount of that digit
                    if n_odd_odds > 0:
                        next_state = states[(n_presented_odds, n_presented_evens, n_odd_odds-1, n_even_evens)]
                        T[next_state][curr_state] += n_odd_odds

                    # Adding an odd digit to numbers having an even amount of that digit
                    if n_odd_odds < n_presented_odds:
                        next_state = states[(n_presented_odds, n_presented_evens, n_odd_odds+1, n_even_evens)]
                        T[next_state][curr_state] += n_presented_odds - n_odd_odds

                    # Adding a new odd digit
                    if n_presented_odds < 5:
                        next_state = states[(n_presented_odds+1, n_presented_evens, n_odd_odds+1, n_even_evens)]
                        T[next_state][curr_state] += 5 - n_presented_odds

                    # Adding an even digit to numbers having an even amount of that digit
                    if n_even_evens > 0:
                        next_state = states[(n_presented_odds, n_presented_evens, n_odd_odds, n_even_evens-1)]
                        T[next_state][curr_state] += n_even_evens

                    # Adding an even digit to numbers having an odd amount of that digit
                    if n_even_evens < n_presented_evens:
                        next_state = states[(n_presented_odds, n_presented_evens, n_odd_odds, n_even_evens+1)]
                        T[next_state][curr_state] += n_presented_evens - n_even_evens

                    # Adding a new even digit
                    if n_presented_evens < 5:
                        next_state = states[(n_presented_odds, n_presented_evens+1, n_odd_odds, n_even_evens)]
                        T[next_state][curr_state] += 5 - n_presented_evens

    sum_index = m-1
    for ans_index in ans_states:
        T[sum_index][ans_index] += 1
    T[sum_index][sum_index] = 1

    S = np.zeros((m, 1), dtype=object)
    S[states[(1, 0, 1, 0)]][0] = 5
    S[states[(0, 1, 0, 0)]][0] = 4

    ans = 0

    curr = 0
    prev = 0

    for k in range(1, N+1):
        print(k)
        curr, prev = 2**k, curr
        step = curr - prev
        S = np.matmul(pow(T, step), S)
        ans += S[sum_index][0]
        ans %= MOD

    print(ans)
