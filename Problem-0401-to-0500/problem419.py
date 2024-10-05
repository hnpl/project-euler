# Question: https://projecteuler.net/problem=419

# http://www.njohnston.ca/2010/10/a-derivation-of-conways-degree-71-look-and-say-polynomial/
# https://mathworld.wolfram.com/CosmologicalTheorem.html
# https://static01.nyt.com/packages/pdf/crossword/GENIUS_AT_PLAY_Eureka_Article.pdf

# There are 92 substrings that, each of them transforms to some substrings within those 92 substrings.
# Use Markov chain to find the number of occurrences of each substring.

import numpy as np

N = 10**12
MOD = 2**30
DTYPE=object

atoms = [
"1112",
"1112133",
"111213322112",
"111213322113",
"1113",
"11131",
"111311222112",
"111312",
"11131221",
"1113122112",
"1113122113",
"11131221131112",
"111312211312",
"11131221131211",
"111312211312113211",
"111312211312113221133211322112211213322112",
"111312211312113221133211322112211213322113",
"11131221131211322113322112",
"11131221133112",
"1113122113322113111221131221",
"11131221222112",
"111312212221121123222112",
"111312212221121123222113",
"11132",
"1113222",
"1113222112",
"1113222113",
"11133112",
"12",
"123222112",
"123222113",
"12322211331222113112211",
"13",
"131112",
"13112221133211322112211213322112",
"13112221133211322112211213322113",
"13122112",
"132",
"13211",
"132112",
"1321122112",
"132112211213322112",
"132112211213322113",
"132113",
"1321131112",
"13211312",
"1321132",
"13211321",
"132113212221",
"13211321222113222112",
"1321132122211322212221121123222112",
"1321132122211322212221121123222113",
"13211322211312113211",
"1321133112",
"1322112",
"1322113",
"13221133112",
"1322113312211",
"132211331222113112211",
"13221133122211332",
"22",
"3",
"3112",
"3112112",
"31121123222112",
"31121123222113",
"3112221",
"3113",
"311311",
"31131112",
"3113112211",
"3113112211322112",
"3113112211322112211213322112",
"3113112211322112211213322113",
"311311222",
"311311222112",
"311311222113",
"3113112221131112",
"311311222113111221",
"311311222113111221131221",
"31131122211311122113222",
"3113112221133112",
"311312",
"31132",
"311322113212221",
"311332",
"3113322112",
"3113322113",
"312",
"312211322212221121123222113",
"312211322212221121123222112",
"32112"
]

atom_index_map = { atom: index for index, atom in enumerate(atoms) }
atom_set = set(atoms)

def next_seq(seq):
    curr_char = ''
    curr_count = 0
    new_seq = []
    for char in seq:
        if char == curr_char:
            curr_count += 1
        else:
            new_seq.append(f'{curr_count}{curr_char}')
            curr_char = char
            curr_count = 1
    new_seq.append(f'{curr_count}{curr_char}')
    new_seq = "".join(new_seq[1:])
    return new_seq

def is_valid(original_seq, partitions): # this is because atoms[81] was incorrectly split ...
    expected_next_seq = original_seq
    actual_partitions = partitions[:]
    for i in range(10):
        expected_next_seq = next_seq(expected_next_seq)
        actual_partitions = [next_seq(part) for part in actual_partitions]
        if not expected_next_seq == "".join([part for part in actual_partitions]):
            return False
    return True

def dfs(depth, max_depth, seq, path):
    if depth >= max_depth:
        return path[:], is_valid(seq, path)

    fully_atomizable = False
    result = path[:]

    next_atom = ''
    for i in range(depth, max_depth):
        next_atom += seq[i]
        if next_atom in atom_set:
            temp_result, fully_atomizable = dfs(i+1, max_depth, seq, result + [next_atom])
            if fully_atomizable:
                result = temp_result
                break

    return result, fully_atomizable

def seq_to_atom(seq):
    return dfs(0, len(seq), seq, [])

def count_1s_2s_3s(seq):
    freq = {'1': 0, '2': 0, '3': 0}
    for char in seq:
        freq[char] += 1
    return freq['1'], freq['2'], freq['3']

def pow(T, exp, mod):
    base = np.array(T, dtype=DTYPE)
    n, m = T.shape
    ans = np.eye(n, dtype=DTYPE)
    while exp > 0:
        if exp % 2 == 1:
            ans = np.matmul(ans, base)
            ans = np.mod(ans, mod)
        base = np.matmul(base, base)
        base = np.mod(base, mod)
        exp //= 2
    return ans


if __name__ == "__main__":
    seq = "1"
    n_atoms = len(atoms)
    fully_atomizable = False # find the first string that appears in the atom set
                             # once the string is in the atom set, the subsequent strings are constructed by the atom set
    count_1s = 0
    count_2s = 0
    count_3s = 0

    n_iters = 0

    first_fully_atomizable_seq = ''

    while True:
        n_iters += 1
        seq = next_seq(seq)
        _, fully_atomizable = seq_to_atom(seq)
        if fully_atomizable:
            first_fully_atomizable_seq = seq
            break
        assert(n_iters < 10)

    neighbors = [[] for i in range(n_atoms)]

    for atom_seq in atom_set:
        new_seq = next_seq(atom_seq)
        sub_atoms, fully_atomizable = seq_to_atom(new_seq)
        assert(fully_atomizable)
        for sub_atom in sub_atoms:
            neighbors[atom_index_map[atom_seq]].append(atom_index_map[sub_atom])

    T = np.zeros((n_atoms, n_atoms), dtype=DTYPE)
    for u in range(n_atoms):
        for v in neighbors[u]:
            T[v][u] += 1
    S = np.zeros((n_atoms, 1), dtype=DTYPE)
    sub_atoms, _ = seq_to_atom(first_fully_atomizable_seq)
    for sub_atom in sub_atoms:
        S[atom_index_map[sub_atom]][0] += 1
    S = np.matmul(pow(T, N - n_iters - 1, MOD), S)
    s = first_fully_atomizable_seq
    for i in range(40-n_iters-1):
        s = next_seq(s)
    S = np.mod(S, MOD)

    for index, atom in enumerate(atoms):
        n_1s, n_2s, n_3s = count_1s_2s_3s(atom)
        if S[index][0] > 0:
            count_1s = int(count_1s + n_1s * S[index][0]) % MOD
            count_2s = int(count_2s + n_2s * S[index][0]) % MOD
            count_3s = int(count_3s + n_3s * S[index][0]) % MOD

    print(f"{count_1s},{count_2s},{count_3s}")
