# Problem: https://projecteuler.net/problem=316

import numpy as np
from sage.all import *

N = 10**6
M = 10**16

class State:

    digits = [str(i) for i in range(0, 10)]

    def __init__(self, target_str, n_matches, probability):
        self.target_str = target_str
        self.n_matches = n_matches
        self.probability = probability

    def generate_next_states(self):
        next_states = []
        for next_digit in State.digits:
            new_str_n = self.target_str[:self.n_matches] + next_digit
            next_n_matches = 0
            if self.target_str[self.n_matches] == next_digit:
                next_n_matches = self.n_matches + 1
            # should have used KMP here
            if self.n_matches > 0 and next_n_matches == 0:
                new_str_n = new_str_n[1:]
                while len(new_str_n) > 0:
                    if self.target_str.startswith(new_str_n):
                        next_n_matches = len(new_str_n)
                        break
                    new_str_n = new_str_n[1:]
            next_states.append(State(self.target_str, next_n_matches, self.probability / len(State.digits)))
        return next_states

def g_absobing_Markov_process(n):
    ans = 0

    n_digits = len(str(n))
    ABSORBING_STATE = n_digits

    T = matrix(QQ, n_digits+1, n_digits+1)
    
    for n_matches in range(0, n_digits):
        state = State(str(n), n_matches, Rational("1/1"))
        next_states = state.generate_next_states()
        for next_state in next_states:
            T[n_matches, next_state.n_matches] += next_state.probability


    rhs = matrix(QQ, n_digits+1,1, lambda i, j: 1)
    rhs[ABSORBING_STATE,0] = 0

    A = matrix(QQ, np.eye(n_digits+1)) - T

    x = A.solve_right(rhs)

    ans = int(round(x[0][0])) - n_digits + 1

    return ans

def reduced_number_form(n):
    next_alphabet = 0
    reduced_str = ""

    alphabet_map = {}

    str_n = str(n)
    for ch in str_n:
        if not ch in alphabet_map:
            alphabet_map[ch] = str(next_alphabet)
            next_alphabet += 1
        reduced_str += alphabet_map[ch]

    return reduced_str

if __name__ == "__main__":
    ans = 0

    cache = {}

    for n in range(2, N):
        a = M//n
        reduced_form = reduced_number_form(a)
        if not reduced_form in cache:
            cache[reduced_form] = g_absobing_Markov_process(reduced_form)
        ans = ans + cache[reduced_form]
        if n % 1000 == 0:
            print(n)


    print(ans)