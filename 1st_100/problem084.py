# Problem: https://projecteuler.net/problem=84

"""
    Use Markov process
"""

import numpy as np
from collections import defaultdict

def get_dice_distribution_two_4_sided_dice():
    freq = {}
    n_combinations = 4*4
    for i in range(1, 5):
        for j in range(1, 5):
            freq[(i, j)] = 1
    for key in freq.keys():
        freq[key] /= n_combinations
    return freq

class State:
    N_STATES = 40

    GO = 0
    J = 10
    G2J = 30
    CC = [2, 17, 33]
    CH = [7, 22, 36]
    R = [5, 15, 25, 35, N_STATES + 5] # added a pseudo one
    U = [12, 28, N_STATES + 12] # added a pseudo one

    # some specific squares
    C1 = 11
    E3 = 24
    H2 = 39
    R1 = 5

    dice_distribution = get_dice_distribution_two_4_sided_dice()

    def __init__(self, n_doubles, position, probability):
        self.n_doubles = n_doubles
        self.position = position
        self.probability = probability

    def get_next_position(self, i):
        return (self.position + i) % self.N_STATES

    def get_next_R(self, current_position):
        i = 0
        while self.R[i] < current_position:
            i += 1
        return self.R[i] % self.N_STATES

    def get_next_U(self, current_position):
        i = 0
        while self.U[i] < current_position:
            i += 1
        return self.U[i] % self.N_STATES

    def get_next_states_from_CC(self):
        next_states = []
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.GO,
                                 probability = self.probability * 1/16))
        next_states.append(State(n_doubles = 0,
                                 position = self.J,
                                 probability = self.probability * 1/16))
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.position,
                                 probability = self.probability * 14/16))
        return next_states

    def get_next_states_from_CH(self, position_of_CH):
        next_states = []

        # Advance to GO
        next_states.append(State(n_doubles = self.n_doubles,
                                     position = self.GO,
                                     probability = self.probability * 1/16))

        # Go to J
        next_states.append(State(n_doubles = 0,
                                 position = self.J,
                                 probability = self.probability * 1/16))

        # Go to C1
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.C1,
                                 probability = self.probability * 1/16))

        # Go to E3
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.E3,
                                 probability = self.probability * 1/16))

        # Go to H2
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.H2,
                                 probability = self.probability * 1/16))

        # Go to R1
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.R1,
                                 probability = self.probability * 1/16))

        # Go to the next R
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.get_next_R(self.position),
                                 probability = self.probability * 2/16))

        # Go to the next U
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.get_next_U(self.position),
                                 probability = self.probability * 1/16))

        # Go back 3 squares
        if not position_of_CH == self.CH[-1]:
        #if True:
            next_states.append(State(n_doubles = self.n_doubles,
                                     position = self.get_next_position(-3), 
                                     probability = self.probability * 1/16))
        else:
            pseudo_next_state = State(n_doubles = self.n_doubles,
                                      position = self.position,
                                      probability = self.probability * 1/16)
            next_states.extend(pseudo_next_state.get_next_states_from_CC())

        # stay at the same position
        next_states.append(State(n_doubles = self.n_doubles,
                                 position = self.position,
                                 probability = self.probability * 6/16))

        return next_states

    def get_next_states(self):
        next_states = []
        for d1, d2 in self.dice_distribution.keys():
            roll_probability = self.dice_distribution[(d1, d2)]
            is_a_double = (d1 == d2)

            if is_a_double and self.n_doubles == 2:
                next_states.append(State(n_doubles = 0,
                                         position = self.J,
                                         probability = self.probability * roll_probability))
                continue

            delta_position = d1 + d2
            next_position = self.get_next_position(delta_position)
            if is_a_double:
                next_n_doubles = self.n_doubles + 1
            else:
                next_n_doubles = 0

            if next_position in self.CC:
                pseudo_next_state = State(n_doubles = next_n_doubles,
                                          position = next_position,
                                          probability = self.probability * roll_probability)
                next_states.extend(pseudo_next_state.get_next_states_from_CC())
            elif next_position in self.CH:
                pseudo_next_state = State(n_doubles = next_n_doubles,
                                          position = next_position,
                                          probability = self.probability * roll_probability)
                next_states.extend(pseudo_next_state.get_next_states_from_CH(next_position))
            elif next_position == self.G2J:
                next_states.append(State(n_doubles = 0,
                                         position = self.J,
                                         probability = self.probability * roll_probability))
            else:
                next_states.append(State(n_doubles = next_n_doubles,
                                         position = next_position,
                                         probability = self.probability * roll_probability))
        return next_states

    def get_hash(self):
        return self.n_doubles * self.N_STATES + self.position

if __name__ == "__main__":
    ans = 0.0

    n_states = 40 * 3

    S = np.zeros(n_states)
    S[0] = 1.0

    T = np.zeros((n_states, n_states), dtype = np.float)

    for pos in range(40):
        for n_doubles in range(3):
            state = State(n_doubles = n_doubles,
                          position = pos,
                          probability = 1.0)
            state_hash = state.get_hash()
            next_states = state.get_next_states()
            for next_state in next_states:
                next_state_hash = next_state.get_hash()
                T[state_hash][next_state_hash] += next_state.probability

    for i in range(200):
        S = S.dot(T)

    sorted_idx = np.argsort(S)[::-1]
    print(sorted_idx[0], S[sorted_idx[0]])
    print(sorted_idx[1], S[sorted_idx[1]])
    print(sorted_idx[2], S[sorted_idx[2]])

    print("Sum S = ", np.sum(S))

    ans = "{}{}{}".format(sorted_idx[0], sorted_idx[1], sorted_idx[2])

    print(ans)