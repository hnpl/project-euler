# Problem: https://projecteuler.net/problem=367

# To run the code: `./problem367.sh`

"""
    * Define notations as follows:
        . let e(i) represent the element at position i of a permutation
        . a permutation p of n elements is represented as follows
            p = [e(1) e(2) e(3) ... e(n)]
        . a permutation p of n elements is called n-cyclic if:
            e(e(e(...e(1)))) = 1
            ^^^^^^^^^^
            `e` appears n times
        . let (e(1) e(2) ... e(n)) represent an n-cyclic permutation.
        . define a histogram as a function f: N -> N such that:
            f(i) = j means the element i appears j times in the data
        . define a `permutation cycle histogram` of permutation p of length n as a function h: N -> N such that
            h(i) = j means the (i-cyclic) sub-permutations appears j times in the permutation p
            where the domain of h is {0, 1, ..., n}

    * Let classify a permutation p using the "permutation cycle histogram".

    * Observation:
        Each permutation p is a collections of cyclic sub-permutations.
        For example,
                permutation | cyclic subpermutations | permutation cycle histogram 
                [3 1 2 4 5] | (1 2 3) (4 5)          | [0 0 1 1 0 0] -> one 2-cyclic subpermutation, one 3 cyclic subpermutation
                [1 2 3 4 5] | (1) (2) (3) (4) (5)    | [0 5 0 0 0 0] -> five 1-cyclic subpermutation
                [2 3 4 5 1] | (1 2 3 4 5)            | [0 0 0 0 0 1] -> one 5-cyclic subpermutation
    
    * Let hash_p(histogram) be a bijection function between all "permutation cycle histograms" and their corresponding hash.
      Let hash(permutation) be hash_p(histogram) where histogram is the "permutation cycle histogram" of the permutation.

    * Consider permutations of N elements.
      Let X = [0 1 ... N-1]

    * The code is as follows,
        - problem367.sh: compiles and runs the solver.
        - problem367.cpp: generates all permutations of X and counts the number of appearances of each possible hash of permutation cycle histograms.
        - problem367.py: it does 3 things
            * generates the transisition matrix:
                - for each possible permutation cycle histogram, it generates a representative permutation that maps to such the histogram
                - for all ways of picking 3 elements and shuffling them, it determines the probability of transitioning to other permutation cycle histograms
            * uses a linear solver to solve the absorbing Matrix chain where:
                - the sorted permutation is the absorbing state
            * calculates the expectation:
                - The linear solver above returns the expected number of steps to reach the absorbing state for all possible starting states.
                - We have the distribution of starting states from problem367.cpp, so we can use the linearity of expectation to calculate the answer to the problem:
                    Expectation = sum{(probability of starting at state S) * (the expected number of steps to reach the absorbing state from S) | for all S in all possible starting states}
"""

from sage.all import *

import itertools
from collections import defaultdict
import numpy as np

def round_up_to_nearest_log_2(n):
    exp = 0
    curr = 1
    while curr < n:
        curr = curr * 2
        exp = exp + 1
    return exp

def mask(n_bits):
    return 2**n_bits - 1

N = 11
M = 3
BITS_PER_ELEMENT = round_up_to_nearest_log_2(N)
ELEMENT_MASK = mask(BITS_PER_ELEMENT)


def dfs1(curr_node, first_node, seq, visited):
    if visited[curr_node]:
        return 0
    visited[curr_node] = True
    return 1 + dfs1(seq[curr_node], first_node, seq, visited)

def find_cycle_length_started_at(first_node, seq, visited):
    return dfs1(first_node, first_node, seq, visited)

def find_cycle_length_freqs(seq):
    length_freq = [0] * (N+1)
    visited = [False] * N
    for first_node in range(N):
        if not visited[first_node]:
            cycle_length = find_cycle_length_started_at(first_node, seq, visited)
            length_freq[cycle_length] += 1
    return length_freq

def hash_freq(freq):
    h = 0
    msb_index = 0

    for length in range(N+1):
        element = freq[length]
        h = h | (element << msb_index)
        msb_index += BITS_PER_ELEMENT

    return h

def hash_seq(seq):
    return hash_freq(find_cycle_length_freqs(seq))

def decode_hash(h):
    length_freqs = []

    for i in range(N+1):
        length_freqs.append(h & mask(ELEMENT_MASK))
        h >>= BITS_PER_ELEMENT

    return length_freqs

def dfs2(curr_node, depth, curr_sum, target_sum, path, ans):
    if curr_sum > target_sum:
        return
    elif curr_sum == target_sum:
        freq = [0] * (N+1)
        for length in path[:depth]:
            freq[length] += 1
        ans.append(freq)
        return
    next_node_lowerbound = curr_node
    for i in range(max(curr_node, 1), target_sum - curr_sum+1):
        path[depth] = i
        dfs2(i, depth+1, curr_sum + i, target_sum, path, ans)

def get_permutation(seq, permutation_index):
    p = [0] * len(seq)
    for p_i, seq_i in enumerate(permutation_index):
        p[p_i] = seq[seq_i]
    return p

def left_rotate_by_k(seq, k):
    return seq[k:] + seq[:k]

def construct_representative_seq(cycle_length_freq):
    seq = []

    cycle_lengths = []
    for length, freq in enumerate(cycle_length_freq):
        for i in range(freq):
            cycle_lengths.append(length)

    first_index = 0

    for length in cycle_lengths:
        seq += get_permutation(list(range(first_index, first_index + length)), left_rotate_by_k(list(range(length)), k=1))
        first_index += length

    return seq

def find_next_states(curr_length_freq):
    next_states = defaultdict(lambda: 0)

    seq = construct_representative_seq(cycle_length_freq)

    for picked_idx in itertools.combinations(list(range(N)), M):
        #for permutation in [[0,1,2], [0,2,1], [1,0,2], [1,2,0], [2,0,1], [2,1,0]]:
        for permutation in itertools.permutations(list(range(M))):
            new_seq = seq[:]
            for i in range(M):
                new_seq[picked_idx[i]] = seq[picked_idx[permutation[i]]]
            h = hash_seq(new_seq)
            next_states[h] += 1

    return next_states

if __name__ == "__main__":
    curr_path = [0] * N
    cycle_length_freqs_sum_to_N = []
    dfs2(0, 0, 0, N, curr_path, cycle_length_freqs_sum_to_N)

    hash_to_index = {}
    max_index = 0
    stopping_state_index = 0
    for cycle_length_freq in cycle_length_freqs_sum_to_N:
        h = hash_freq(cycle_length_freq)
        hash_to_index[h] = max_index
        if cycle_length_freq[1] == N:
            stopping_state_index = max_index
        max_index += 1

    n = len(cycle_length_freqs_sum_to_N)
    T = matrix(QQ, n, n)

    for cycle_length_freq in cycle_length_freqs_sum_to_N:
        curr_state_hash = hash_freq(cycle_length_freq)
        next_states = find_next_states(cycle_length_freq)
        n_next_states = sum(next_states.values())
        if hash_to_index[curr_state_hash] == stopping_state_index:
            #T[stopping_state_index, stopping_state_index] = 0
            continue
        for next_state_hash, freq in next_states.items():
            T[hash_to_index[curr_state_hash], hash_to_index[next_state_hash]] += QQ("{}/{}".format(freq, n_next_states))

    S = matrix(QQ, 1, n)
    with open("p367_count.txt") as f:
        for line in f.readlines():
            h, freq = list(map(int, line.strip().split(" ")))
            S[0, hash_to_index[h]] = QQ("{}/{}".format(freq, factorial(N)))

    # Absorbing Markov Chain
    ABSORBING_STATE = stopping_state_index
    rhs = matrix(QQ, n, 1, lambda i, j: 1)
    rhs[ABSORBING_STATE, 0] = 0

    A = matrix(QQ, np.eye(n)) - T

    x = A.solve_right(rhs)

    expectation = sum(S * x)[0]

    print(ceil(expectation))
   
