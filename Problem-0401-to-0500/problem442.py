# Question: https://projecteuler.net/problem=442

import numpy as np
from math import ceil, log

N = 10**18
#N = 213
#N = 531563
N_DIGITS = int(ceil(log(N)/log(10)))

class StateDescription:
    def __init__(self, suffix):
        self.suffix = suffix[:]
    # special state: suffix == "-1" -> the string always has pow_11_substring
    def has_pow_11_substring(self):
        return self.suffix == "-1"
    # special state: suffix == "-2" -> the string does have pow_11_substring so far, and the suffix is not the prefix of any pow_11
    def not_have_pow_11_substring_yet(self):
        return self.suffix == "-2"
    def is_special_state(self):
        return self.suffix[0] == "-"
    def __str__(self):
        return f"({self.suffix})"
    def hash(self):
        return self.suffix

def get_S_T(starting_prefix):
    exp_11 = [str(11**j) for j in range(1, N_DIGITS) if j != 11 and j != 13] # 11**11 has a substring of "11" and 11**13 has a substring of "121"
    states = {}
    starting_prefix = str(starting_prefix)
    ps = []

    pow_prefix_index = {}

    states["-1"] = StateDescription("-1")
    states["-2"] = StateDescription("-2")

    k = 1
    for exp in exp_11:
        k += 1
        for i in range(1, len(exp)):
            state = StateDescription(exp[:i])
            states[state.hash()] = state

    n_states = len(states)
    T = np.zeros((n_states, n_states), dtype = np.uint64)

    idx = 0
    for suffix, _ in states.items(): # suffix -> current string suffix; prefix -> power of 11's prefix
        pow_prefix_index[suffix] = idx
        idx += 1

    # next states
    for curr_prefix, curr_prefix_index in pow_prefix_index.items():
        if curr_prefix.startswith("-1"): # this is the absorbing state
            T[curr_prefix_index][curr_prefix_index] += 10
            continue
        if curr_prefix.startswith("-"):
            curr_prefix = ""
        for next_digit in range(10):
            next_prefix = f"{curr_prefix}{next_digit}"
            next_state_hash = ""
            if next_prefix in exp_11:
                next_state_hash = "-1"
            elif next_prefix in pow_prefix_index:
                next_state_hash = next_prefix[:]
            else:
                assigned = False
                for k in range(len(next_prefix)):
                    next_prefix_suffix = next_prefix[k:]
                    if next_prefix_suffix in exp_11:
                        next_state_hash = "-1"
                        assigned = True
                        break
                    elif next_prefix_suffix in pow_prefix_index:
                        next_state_hash = next_prefix_suffix[:]
                        assigned = True
                        break
                if not assigned:
                    next_state_hash = "-2"
            next_prefix_index = pow_prefix_index[next_state_hash]
            T[next_prefix_index][curr_prefix_index] += 1

    # initial state
    initial_S = np.zeros((n_states, 1), dtype = np.uint64)
    initialized = False
    if starting_prefix == "0":
        prefix = "-2"
        prefix_index = pow_prefix_index[prefix]
        initial_S[prefix_index, 0] = 1
        initialized = True
    if not initialized:
        for exp in exp_11:
            if exp in starting_prefix:
                prefix = "-1"
                prefix_index = pow_prefix_index[prefix]
                initial_S[prefix_index, 0] = 1
                initialized = True
                break
    if not initialized:
        for i in range(len(starting_prefix)):
            prefix = starting_prefix[i:]
            if prefix in pow_prefix_index:
                prefix_index = pow_prefix_index[prefix]
                initial_S[prefix_index, 0] = 1
                initialized = True
                break
    if not initialized:
        prefix = "-2"
        prefix_index = pow_prefix_index[prefix]
        initial_S[prefix_index, 0] = 1
    answer_indices = set(range(n_states))
    prefix = "-1"
    prefix_index = pow_prefix_index[prefix]
    answer_indices = {prefix_index}

    return initial_S, T, answer_indices

def pow(a, exp):
    n, _ = a.shape

    base = np.array(a, dtype=np.uint64)
    ans = np.eye(n, dtype = np.uint64)

    while exp > 0:
        if exp % 2 == 1:
            ans = np.matmul(ans, base)
        exp //= 2
        base = np.matmul(base, base)

    return ans

class Config:
    cache = {}
    def __init__(self, prefix, n_remaining):
        self.prefix = str(prefix)
        self.n_remaining = n_remaining
    def get_hash(self):
        return f"{self.prefix},{self.n_remaining}"
    def calculate(self):
        hash = self.get_hash()
        if not hash in Config.cache:
            # main calculation
            S, T, answer_indices = get_S_T(self.prefix)
            S = np.matmul(pow(T, self.n_remaining), S)
            ans = np.array([0], dtype=np.uint64)
            for idx in answer_indices:
                ans[0] += S[idx, 0]
            Config.cache[hash] = ans[0]
        return Config.cache[hash]

def count_numbers_containing_pow_11_substring(n):
    ans = np.array([0], dtype=np.uint64)
    n_digits = int(ceil(log(n)/log(10)))
    most_significant_digit = n // (10**(n_digits-1))
    for first_digit in range(most_significant_digit):
        prefix = first_digit
        n_remaining_digits = n_digits - 1
        config = Config(prefix, n_remaining_digits)
        ans[0] += config.calculate()
        #S, T, answer_indices = get_S_T(prefix)
        #print("prefix:", prefix)
        #print(S)
        #print(T)
        #S = np.matmul(pow(T, n_remaining_digits), S)
        #print(S)
        #for idx in answer_indices:
        #    ans[0] += S[idx, 0]
        #print(ans)
        #print("-----")
    for j in range(2, n_digits+1):
        prefix = n // (10**(n_digits-j))
        #print("PREFIX", prefix)
        prefix_range_start = prefix // 10
        n_remaining_digits = n_digits - j
        for p in range(prefix_range_start * 10, prefix):
            config = Config(p, n_remaining_digits)
            ans[0] += config.calculate()
            #S, T, answer_indices = get_S_T(p)
            #print("prefix:", p)
            #print(S)
            #print(T)
            #print(answer_indices)
            #S = np.matmul(pow(T, n_remaining_digits), S)
            #for idx in answer_indices:
            #    ans[0] += S[idx, 0]
            #print(S)
            #print(ans)
            #print("-----")
    return ans

if __name__ == "__main__":
    diff = 0
    l = N
    r = 2**64-1
    while l < r:
        m = (l + r) // 2
        diff = m - count_numbers_containing_pow_11_substring(m)
        if diff > N:
            r = m - 1
        else:
            l = m
        print(l, r, m)
    print(l)
    
