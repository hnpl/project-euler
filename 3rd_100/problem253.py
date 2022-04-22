# Question: https://projecteuler.net/problem=253

"""
    Strategy:
        - Keep track of "holes" length histogram (hole = the space between two number)
            + e.g. suppose we have 10 pieces, and we filled the range [3,4] and [7,8], then the holes are,
            [1,2], [5,7], [9,10]
            + The holes length are,
                2,     3,      2
            + The histogram is,
            length: count
                 1: 0
                 2: 2
                 3: 1
                 4: 0
                 5: 0
        - We also keep track of the maximum number of segments per state.
        - Then for each state, we try to add one number to each hole to find the new states.
"""

N = 40

class State:
    def __init__(self, num_pieces, length_freq, max_num_segments):
        self.num_pieces = num_pieces
        self.length_freq = length_freq[:]
        self.max_num_segments = max_num_segments
        self.hash = self.get_hash()
    def get_initial_state(num_pieces):
        length_freq = [0] * (num_pieces+1)
        length_freq[num_pieces] = 1
        max_num_segments = 1
        return State(num_pieces, length_freq, max_num_segments)
    def to_str(self):
        return f"{self.length_freq[1:]} max: {self.max_num_segments}"
    def get_hash(self):
        hash = 0
        #num_bits_per_length = log2(max_num_lengths)
        num_bits_per_length = 5
        msb = 0
        num_lengths = len(self.length_freq)
        for length in range(1, num_lengths):
            hash = hash | (self.length_freq[length] << msb)
            msb += num_bits_per_length
        hash = hash | (self.max_num_segments << msb)
        return hash
    def get_next_states(self):
        next_states = []
        new_length_freq = self.length_freq[:]
        n_ways = 0
        for length, freq in enumerate(self.length_freq[1:], start=1):
            if freq == 0:
                continue
            if length == 1:
                new_length_freq[length] -= 1
                n_ways = freq
                next_states.append((State(self.num_pieces, new_length_freq, self.max_num_segments), freq))
                new_length_freq[length] += 1
                continue
            new_length_freq[length] -= 1
            for new_length_1 in range(length):
                new_length_2 = length - new_length_1 - 1
                new_length_freq[new_length_1] += 1
                new_length_freq[new_length_2] += 1
                next_states.append((State(self.num_pieces, new_length_freq, max(self.max_num_segments, sum(new_length_freq[1:]))), freq))
                new_length_freq[new_length_2] -= 1
                new_length_freq[new_length_1] -= 1
            new_length_freq[length] += 1
        return next_states

if __name__ == "__main__":
    ans = 0
    initial_state = State.get_initial_state(N)
    initial_state_hash = initial_state.hash
    curr_states = {}
    next_states = {initial_state_hash: [initial_state, 1]}

    M_freq = [0] * (N+1)

    for _ in range(N):
        print(_)
        curr_states, next_states = next_states, {}
        for curr_state_hash, curr_state_info in curr_states.items():
            curr_state, curr_state_count = curr_state_info
            for next_state, next_state_count in curr_state.get_next_states():
                next_state_hash = next_state.hash
                if not next_state_hash in next_states:
                    next_states[next_state_hash] = [next_state, 0]
                next_states[next_state_hash][1] += next_state_count * curr_state_count

    for next_state_hash, next_state_info in next_states.items():
        next_state, next_state_count = next_state_info
        M_freq[next_state.max_num_segments] += next_state_count

    for M, freq in enumerate(M_freq):
        ans += M * freq

    from math import factorial
    ans /= factorial(N)
   
    print(M_freq)
    print(ans)
