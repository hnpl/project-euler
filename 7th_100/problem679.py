# Problem: https://projecteuler.net/problem=679

"""
    . Keep track of the last 3 characters
"""

import numpy as np
from collections import defaultdict

MASKS = {0:0, 1: 0b1, 2: 0b11, 3: 0b111}
N = 30
KEYWORDS = {"FREE", "FARE", "AREA", "REEF"}

class State:
    character_encoding = {"0": 0, "A": 1, "E": 2, "F": 3, "R": 4}
    character_decoding = {val: key for key, val in character_encoding.items()}

    def __init__(self):
        pass
        
    def init_from_state(self, has_FREE, has_FARE, has_AREA, has_REEF, last_3_characters):
        self.has_FREE = has_FREE
        self.has_FARE = has_FARE
        self.has_AREA = has_AREA
        self.has_REEF = has_REEF
        self.last_3_characters = last_3_characters
        self.hash = self.encode(has_FREE, has_FARE, has_AREA, has_REEF, last_3_characters)
        return self

    def init_from_hash(self, state_hash):
        self.hash = state_hash
        self.has_FREE, self.has_FARE, self.has_AREA, self.has_REEF, self.last_3_characters = self.decode()
        return self

    def has_all_keywords(self):
        return all([self.has_FREE, self.has_FARE, self.has_AREA, self.has_REEF])

    def encode(self, has_FREE, has_FARE, has_AREA, has_REEF, last_3_characters):
        h = 0
        msb_index = 0

        h = h | (has_FREE << msb_index)
        msb_index += 1

        h = h | (has_FARE << msb_index)
        msb_index += 1

        h = h | (has_AREA << msb_index)
        msb_index += 1

        h = h | (has_REEF << msb_index)
        msb_index += 1

        for ch in last_3_characters:
            h = h | (State.character_encoding[ch] << msb_index)
            msb_index += 3

        return h

    def decode(self):
        h = self.hash

        has_FREE = (h & MASKS[1]) == 1
        h >>= 1

        has_FARE = (h & MASKS[1]) == 1
        h >>= 1

        has_AREA = (h & MASKS[1]) == 1
        h >>= 1

        has_REEF = (h & MASKS[1]) == 1
        h >>= 1

        last_3_characters = ""
        for i in range(3):
            ch = State.character_decoding[(h & MASKS[3])]
            last_3_characters += ch
            h >>= 3

        return has_FREE, has_FARE, has_AREA, has_REEF, last_3_characters

    def get_hash(self):
        return self.hash

    def get_next_states(self):
        next_states = []

        for next_character in ["A", "E", "F", "R"]:
            next_last_4_characters = self.last_3_characters + next_character
            is_valid = True
            if self.has_FREE and next_last_4_characters == "FREE":
                is_valid = False
            if self.has_FARE and next_last_4_characters == "FARE":
                is_valid = False
            if self.has_AREA and next_last_4_characters == "AREA":
                is_valid = False
            if self.has_REEF and next_last_4_characters == "REEF":
                is_valid = False
            if not is_valid:
                continue

            next_has_FREE = self.has_FREE or next_last_4_characters == "FREE"
            next_has_FARE = self.has_FARE or next_last_4_characters == "FARE"
            next_has_AREA = self.has_AREA or next_last_4_characters == "AREA"
            next_has_REEF = self.has_REEF or next_last_4_characters == "REEF"
            next_last_3_characters = next_last_4_characters[-3:]
            next_states.append(State().init_from_state(has_FREE = next_has_FREE,
                                                       has_FARE = next_has_FARE,
                                                       has_AREA = next_has_AREA,
                                                       has_REEF = next_has_REEF,
                                                       last_3_characters = next_last_3_characters))

        return next_states

if __name__ == "__main__":
    ans = 0

    prev_states_freq = None
    curr_states_freq = defaultdict(lambda: 0)
    curr_states_freq[0] = 1

    for length in range(1, N+1):
        prev_states_freq, curr_states_freq = curr_states_freq, defaultdict(lambda: 0)
        for prev_state_hash, prev_state_freq in prev_states_freq.items():
            next_states = State().init_from_hash(prev_state_hash).get_next_states()
            for next_state in next_states:
                next_state_hash = next_state.get_hash()
                curr_states_freq[next_state_hash] += prev_state_freq


    for state_hash, freq in curr_states_freq.items():
        if State().init_from_hash(state_hash).has_all_keywords():
            ans += freq

    print(ans)
