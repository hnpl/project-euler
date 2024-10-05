# Question: https://projecteuler.net/problem=369

"""
    - There are 4 suits. Assign 1 to suit 1,
                                2 to suit 2,
                                3 to suit 3,
                                4 to suit 4.
    - Let the patterns indicate the suits of a number in a hand.
      For example, if we have a hand consisting of 2 (suit 1), 2 (suit 2), 2 (suit 4), 3 (suit 2), 5 (suit 3), 8 (suit 1), 8 (suit 2), 8 (suit 4), 9 (suit 3), 9 (suit 4)
        Then, 2 and 8 is categorized as PATTERN_124,
              3 is categorized as PATTERN_2,
              5 is categorized as PATTERN_3,
              9 is categorized as PATTERN_34
    - We encode a hand as a histogram of each pattern.
      For the hand above, we have this histogram,
       PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4, PATTERN_12, PATTERN_13, PATTERN_14, PATTERN_23, PATTERN_24, PATTERN_34, PATTERN_123, PATTERN_124, PATTERN_134, PATTERN_234, PATTERN_1234
Hash =[        0,         1,         1,         0,          0,          0,          0,          0,          0,          1,           0,           2,           0,           0,            0]
      Each element is <= 13, so we'll use 4 bits per pattern.
    - From each hash, we can figure out the next hashes by adding a card,
      - We can add one card of a number that does not appear in the pattern yet (i.e., adding 1 to one of PATTERN_1, PATTERN_2, PATTERN_3, PATTERN_4).
      - We can add one card of existing pattern,
        e.g., in the above hash, we can add a card of suit 2 to the number having PATTERN_3, then that number is now categorized as PATTERN_23 instead of PATTERN_3, i.e. PATTERN_3 becomes 0 and PATTERN_23 becomes 1.
    - Starting from an initial state of having no cards, we generate new hashes by adding 1 card to previous hashes, as described above.
    - How to know if a hash can result in a badugi? This is an assignment problem (https://en.wikipedia.org/wiki/Assignment_problem) / bipartite matching problem on unweighted graphs.
      - We have 4 suits, and N cards that can fill one slot of each suit.
      - We need to find a matching that each of the 4 suits has a card assigned to.
      - We will use the linear_sum_assignment solver from scipy (https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.linear_sum_assignment.html).
    - For each of the hash, can we count how many different ways of constructing a hand producing that hash? Yes!
      - Note that for each hash, there is no number corresponding to two different patterns.
      - So, basically, we need to count how many ways to fill in numbers in the set {1,2,...,13} to the patterns.
      - We can use multinomial coefficient/theorem to calculate the number of ways of constructing a hand producing a hash.
      - For the above example, we apply the multinomial theorem for 5 objects, with groups of sizes (1, 1, 1, 2).
              
"""

from scipy.optimize import linear_sum_assignment as LSA
import numpy as np
from math import factorial

N = 13

def PatternHash(p):
    return p[0] + p[1]*2 + p[2]*4 + p[3]*8

cached_factorial = {i: factorial(i) for i in range(N+1)}

class Pattern:
    PATTERN_1 = 0
    PATTERN_2 = 1
    PATTERN_3 = 2
    PATTERN_4 = 3
    PATTERN_12 = 4
    PATTERN_13 = 5
    PATTERN_14 = 6
    PATTERN_23 = 7
    PATTERN_24 = 8
    PATTERN_34 = 9
    PATTERN_123 = 10
    PATTERN_124 = 11
    PATTERN_134 = 12
    PATTERN_234 = 13
    PATTERN_1234 = 14

    ALL = list(range(15))

    PATTERN_MAP = [[1,0,0,0],
                   [0,1,0,0],
                   [0,0,1,0],
                   [0,0,0,1],
                   [1,1,0,0],
                   [1,0,1,0],
                   [1,0,0,1],
                   [0,1,1,0],
                   [0,1,0,1],
                   [0,0,1,1],
                   [1,1,1,0],
                   [1,1,0,1],
                   [1,0,1,1],
                   [0,1,1,1],
                   [1,1,1,1]]

    PATTERN_INVERSE_MAP = { PatternHash([1,0,0,0]): 0,
                            PatternHash([0,1,0,0]): 1,
                            PatternHash([0,0,1,0]): 2,
                            PatternHash([0,0,0,1]): 3,
                            PatternHash([1,1,0,0]): 4,
                            PatternHash([1,0,1,0]): 5,
                            PatternHash([1,0,0,1]): 6,
                            PatternHash([0,1,1,0]): 7,
                            PatternHash([0,1,0,1]): 8,
                            PatternHash([0,0,1,1]): 9,
                            PatternHash([1,1,1,0]): 10,
                            PatternHash([1,1,0,1]): 11,
                            PatternHash([1,0,1,1]): 12,
                            PatternHash([0,1,1,1]): 13,
                            PatternHash([1,1,1,1]): 14
                          }
    
    N_PATTERNS = 15

class State:
    def __init__(self):
        self.pattern_counts = [0 for _ in range(Pattern.N_PATTERNS)]
        self.hash = 0

    def init_from_hash(self, hash):
        self.pattern_counts = [0 for _ in range(Pattern.N_PATTERNS)]
        _hash = hash
        for i in range(Pattern.N_PATTERNS):
            self.pattern_counts[i] = _hash & 0xF
            _hash >>= 4
        self.hash = hash

    def init_from_other_pattern_counts(self, pattern_counts):
        self.pattern_counts = pattern_counts[:]
        self.hash = self.get_hash()
        return self

    def get_hash(self):
        _hash = 0
        msb = 0
        for i in range(Pattern.N_PATTERNS):
            _hash = _hash | (self.pattern_counts[i] << msb)            
            msb += 4
        return _hash

    def has_badugi(self):
        n = sum(self.pattern_counts)
        M = np.zeros((n, 4), dtype=np.int8)
        w = 0
        for pattern, count in enumerate(self.pattern_counts):
            for _ in range(count):
                M[w] = Pattern.PATTERN_MAP[pattern]
                w += 1
        rows, cols = LSA(-M) # see problem 345
        return M[rows, cols].sum() == 4

    def get_next_states(self): # what do we get when we add another card
        next_states = []
        for card_to_add in [0,1,2,3]:
            new_pattern_counts = self.pattern_counts[:]
            for initial_pattern in Pattern.ALL: # choosing the category to add to
                                                # e.g. if we add a card of suit 1 to PATTERN_23, it'll become PATTERN_123
                if new_pattern_counts[initial_pattern] > 0 and Pattern.PATTERN_MAP[initial_pattern][card_to_add] == 0:
                    new_pattern = Pattern.PATTERN_MAP[initial_pattern][:]
                    new_pattern[card_to_add] += 1
                    new_pattern_index = Pattern.PATTERN_INVERSE_MAP[PatternHash(new_pattern)]
                    new_pattern_counts[initial_pattern] -= 1
                    new_pattern_counts[new_pattern_index] += 1
                    new_state = State().init_from_other_pattern_counts(new_pattern_counts)
                    next_states.append(new_state)
                    new_pattern_counts[new_pattern_index] -= 1
                    new_pattern_counts[initial_pattern] += 1
        
        # Try to add numbers that are not in the deck yet
        new_pattern_counts = self.pattern_counts[:]
        for pattern in [Pattern.PATTERN_1, Pattern.PATTERN_2, Pattern.PATTERN_3, Pattern.PATTERN_4]:
            new_pattern_counts[pattern] += 1
            new_state = State().init_from_other_pattern_counts(new_pattern_counts)
            next_states.append(new_state)
            new_pattern_counts[pattern] -= 1

        return next_states

    def to_str(self):
        return f"State = {self.pattern_counts}, has_badugi = {self.has_badugi()}"

if __name__ == "__main__":
    initial_state = State()
    initial_state.count = 1
    next_states = initial_state.get_next_states()

    counts = [0] * (N+1)
    curr_states = {}
    next_states = {initial_state.get_hash(): initial_state}

    for n in range(1, N+1):
        curr_states, next_states = next_states, {}
        for curr_state_hash, curr_state in curr_states.items():
            for next_state in curr_state.get_next_states():
                next_state_hash = next_state.get_hash()
                if not next_state_hash in next_states:
                    next_states[next_state_hash] = next_state
        if n >= 4:
            for next_state_hash, next_state in next_states.items():
                if next_state.has_badugi():
                    #n_choices = 13
                    #count = 1
                    #for cat in range(15):
                    #        for _ in range(next_state.cat_counts[cat]):
                    #            count *= n_choices
                    #            n_choices -= 1
                    count = cached_factorial[13] // cached_factorial[13 - sum(next_state.pattern_counts)]
                    for coeff in next_state.pattern_counts:
                        if coeff > 0:
                            count //= cached_factorial[coeff]
                    counts[n] += count
    
    ans = sum(counts[4:])
    print(ans)
