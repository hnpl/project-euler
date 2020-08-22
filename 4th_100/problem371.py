# Problem: https://projecteuler.net/problem=371

"""
    Observation 1: plate 000 is not in any winning combinations.
    Observation 2: plate 500 requires another plate 500 to win; any plate other than 500 requires a different plate to win.
    Observation 3: we don't need to keep track of what plates were seen, we can determine the probability of winning by keeping track of:
                        - whether plate 500 has been seen
                        - the amount of plates other than 000 and 500 were seen
"""

N = 1000

MASK_1  = 0x1
MASK_10 = 0x3FF

class State:
    def __init__(self, has_500, n_plates_not_500_and_000, probability, is_stopping_state):
        self.has_500 = has_500
        self.n_plates_not_500_and_000 = n_plates_not_500_and_000
        self._hash = self.encode(has_500, n_plates_not_500_and_000)
        self._probability = probability
        self._is_stopping_state = is_stopping_state

    def encode(self, has_500, n_plates_not_500_and_000):
        h = 0
        msb_index = 0
        if has_500:
            h = 1
        msb_index += 1
        h = h | (n_plates_not_500_and_000 << msb_index)
        return h

    def get_next_states(self):
        next_states = []

        # seeing 000 plate
        next_state = State(has_500 = self.has_500,
                           n_plates_not_500_and_000 = self.n_plates_not_500_and_000,
                           probability = self._probability / N,
                           is_stopping_state = False)
        next_states.append(next_state)
        # assume that the driver has seen at least one plate that is not 000 (can be 500 or not)

        # if the next is 500
        next_state = State(has_500 = True,
                           n_plates_not_500_and_000 = self.n_plates_not_500_and_000,
                           probability = self._probability / N,
                           is_stopping_state = self.has_500) # is_stopping_state if 500 is seen, not a stopping state otherwise
        next_states.append(next_state)

        # if the next is not 500 and not 000
        # -- if the next is seen (cannot win now, since if the driver wins, they already saw the winning combination in one of the earlier iterations)
        #if self.n_plates_not_500_and_000 > 0:
        next_state = State(has_500 = self.has_500,
                           n_plates_not_500_and_000 = self.n_plates_not_500_and_000,
                           probability = self._probability * (self.n_plates_not_500_and_000 / N),
                           is_stopping_state = False)
        next_states.append(next_state)

        # -- if the next is not seen before
        # ---- if the next does not lead to winning
        next_state = State(has_500 = self.has_500,
                           n_plates_not_500_and_000 = self.n_plates_not_500_and_000 + 1,
                           probability = self._probability * (998 - self.n_plates_not_500_and_000 - self.n_plates_not_500_and_000) / N,
                           #                                        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                           #                                        minus the ones that are seen    minus the ones that lead to winning
                           is_stopping_state = False)
        next_states.append(next_state)

        # ---- if the next leads to winning
        next_state = State(has_500 = self.has_500,
                           n_plates_not_500_and_000 = self.n_plates_not_500_and_000 + 1,
                           probability = self._probability * (self.n_plates_not_500_and_000 / N),
                           is_stopping_state = True)
        next_states.append(next_state)

        return next_states

    def get_hash(self):
        return self._hash
    
    def get_probability(self):
        return self._probability

    def add_probability(self, rhs):
        self._probability += rhs

    def is_stopping_state(self):
        return self._is_stopping_state

if __name__ == "__main__":
    ans = 0.0

    initial_state = State(has_500 = False,
                          n_plates_not_500_and_000 = 0,
                          probability = 1.0,
                          is_stopping_state = False)
    initial_state_hash = initial_state.get_hash()

    next_states = { initial_state_hash: initial_state }
    prev_states = {}

    prev_probability = 0.0
    curr_probability = 1.0
    tol = 1e-9

    n_plates = 0
    
    diff = 1.0

    while diff > tol or (prev_probability + curr_probability) < 1 - tol:
        n_plates = n_plates + 1
        prev_states, next_states = next_states, {}
        curr_probability = 0.0
        for prev_state_hash, prev_state in prev_states.items():
            curr_states = prev_state.get_next_states()
            for curr_state in curr_states:
                if curr_state.is_stopping_state():
                    curr_probability += curr_state.get_probability()
                else:
                    curr_state_hash = curr_state.get_hash()
                    if curr_state_hash in next_states:
                        next_states[curr_state_hash].add_probability(curr_state.get_probability())
                    else:
                        next_states[curr_state_hash] = curr_state

        # print(n_plates, prev_probability + curr_probability, ans)

        prev_probability += curr_probability 

        ans += n_plates * curr_probability

        diff = n_plates * curr_probability

    print(ans)
    

