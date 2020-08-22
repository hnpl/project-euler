# Problem: https://projecteuler.net/problem=458

"""
    . Keep track of the last 6 digits.

    . Build a transition matrix, where T[hash1][hash2] = K means there are K ways to append a digit to
    a sequence of 6 digits (corresponding to hash1) without creating a permutation of {project}, and
    and the hash of the last 6 digits of the new sequence is hash2.

    . To reduce the state space, we define a hash function such that,
        - For two sequences seq1 and seq2 such that each has 6 digits, they have the same hash when
        there exists f such that
            * seq1[i] == seq1[j] iff f[i] == f[j]
            * seq2[i] == seq2[j] iff f[i] == f[j]
        - For example, seq1 = 12332 and seq2 = 51661 have the same has since there such f exist, for example,
            f = "ABCCB"

    . Let S be an array where S[hash1] is a number of ways to construct a 6-digit sequence having the hash
    of hash1.

    . Let S_f be an array where S_f[hash1] is a number of ways to construct a 10**12-digit with
    the last 6 digits corresponding to the hash hash1.

    . Then S_f = S * (T ** (10**12)).

    . Note that T**(10**12) is big, so we calculate S_f using S and T**(10**12) in the modulo ring of 2**9
    and 5**9 separately, and then use Chinese Remainder Theorem to find sum(S_f) mod 10**9.
"""

from sage.all import *

from collections import defaultdict

N = 7
M = 10**12
L = N - 1

def encode(permutation):
    alphabet_map = [-1] * N
    next_alphabet = 0
    for i in range(L):
        if alphabet_map[permutation[i]] < 0:
            alphabet_map[permutation[i]] = next_alphabet
            next_alphabet += 1
    _hash = 0
    msb_index = 0
    for n in permutation:
        _hash = _hash | (alphabet_map[n] << msb_index)
        msb_index += 3
    return _hash

def decode(_hash):
    ans = [0] * L
    msb_index = 0;
    for i in range(L):
        ans[i] = _hash & 0x7
        _hash >>= 3
    return ans

def DFS(depth, path, hash_freq_map):
    if depth == L:
        _hash = encode(path)
        hash_freq_map[_hash] += 1
    else:
        for i in range(N):
            path[depth] = i
            DFS(depth+1, path, hash_freq_map)

def generate_next_states(_hash):
    original_permutation = decode(_hash)
    n_digit_types = len(set(original_permutation))

    next_permutation = decode(_hash>>3)
    next_states = defaultdict(lambda: 0)
    for next_digit in range(N):
        if n_digit_types == N - 1 and not next_digit in original_permutation: #invalid, a permutation of project
            continue
        next_permutation[-1] = next_digit
        next_state = encode(next_permutation)
        next_states[next_state] += 1
    return next_states

if __name__ == "__main__":
    ans = 0

    path = [-1] * L
    hash_freq_map = defaultdict(lambda: 0)

    # discover state space, the result is in hash_freq_map
    DFS(0, path, hash_freq_map)

    n = len(hash_freq_map)

    index_hash_map = list(hash_freq_map.keys())
    hash_index_map = { _hash: index for index, _hash in enumerate(index_hash_map) }

    # state count array
    S = matrix(1, n, 0)
    for _hash, freq in hash_freq_map.items():
        S[0, hash_index_map[_hash]] = freq

    # transistion matrix
    T = matrix(n, n, 0)
    for _hash, freq in hash_freq_map.items():
        next_states = generate_next_states(_hash)
        _hash_index = hash_index_map[_hash]
        for next_hash, next_hash_freq in next_states.items():
            T[_hash_index, hash_index_map[next_hash]] = next_hash_freq

    # S_f = S * (T ** M) for modulo ring 5**9
    R = IntegerModRing(5**9)
    T_5 = matrix(R, T)
    S_5 = matrix(R, S)
    S_5 = S_5 * (T_5 ** (M-6))
    sum_5 = sum(sum(S_5))

    # S_f = S * (T ** M) for modulo ring 2**9
    R = IntegerModRing(2**9)
    T_2 = matrix(R, T)
    S_2 = matrix(R, S)
    S_2 = S_2 * (T_2 ** (M-6))
    sum_2 = sum(sum(S_2))

    # use Chinese Remainder Theorem to calculate "ans mod 10**9"
    ans = crt(lift(sum_2), lift(sum_5), 2**9, 5**9)

    print(ans)