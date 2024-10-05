# Question: https://projecteuler.net/problem=612

# - Strategy:
#   . We can define a state as the used digits (except for leading zeros). We can hash the state as state_bit[k] = 1 when digit k is used, and state_bit[k] = 0 otherwise.
#   . The state space is 2**10.
#   . We can derive a 1024x1024 transition matrix T by trying adding a new digit to every state.
#   . Define S(n) as a vector counting the appearances of each state for numbers with exactly n digits.
#       . S(n+1) = T*S(n)
#   . Define final_S(n) as a vector counting the appearances of each state for numbers with equal or fewer than n digits.
#       . final_S(n+1) = final_S(n) + S(n+1)
#   . To avoid counting 01 as using 0, we start with states of 1 digit except for 0, then try adding new digits to the right of each state.
#   . It's easier to count non-friend pairs.
#       . The number of non-friend pairs is the sum of final_S(n+1)[state=alpha] * final_S(n+1)[state=beta] for all alpha and beta such that logical-AND(alpha_hash, beta_hash) == 0
#   . So let K be the number of non-friend pairs, then the answer is the number of possible pairs minus the number of non-friend pairs.
#       . i.e., ans = (10**N-1) * (10**N-2) / 2 - K
import numpy as np

N = 18 # 10^18 -> 18 digits
MOD = 1000267129

class State:
    def __init__(self, used_digits):
        self.hash = State.calculate_hash(used_digits)
    def init_from_hash(_hash):
        state = State([])
        state.hash = _hash
        return state
    def get_hash(self):
        return self.hash
    def get_initial_states():
        initial_states = []
        for first_digit in [1,2,3,4,5,6,7,8,9]:
            initial_states.append(State([first_digit]))
        return initial_states
    def calculate_hash(used_digits):
        return sum([2**k for k in used_digits])
    def get_next_states(self):
        next_states = []
        for next_digit in [0,1,2,3,4,5,6,7,8,9]:
            next_state_hash = self.hash | (1 << next_digit)
            next_states.append(State.init_from_hash(next_state_hash))
        return next_states

def matrix_pow_mod(base, exp, mod):
    n, m = base.shape
    ans = np.identity(n, dtype = np.uint64)
    base_pow = np.array(base, dtype=np.uint64)
    while exp > 0:
        if exp % 2 == 1:
            ans = np.matmul(ans, base_pow)
            ans = np.mod(ans, mod)
        exp //= 2
        base_pow = np.matmul(base_pow, base_pow)
        base_pow = np.mod(base_pow, mod)
    return ans

if __name__ == "__main__":
    state_space_size = 2**10
    T = np.zeros((state_space_size, state_space_size), dtype=np.uint64)
    for curr_state_hash in range(state_space_size):
        curr_state = State.init_from_hash(curr_state_hash)
        for next_state in curr_state.get_next_states():
            next_state_hash = next_state.get_hash()
            T[next_state_hash][curr_state_hash] += 1
    S = np.zeros((state_space_size, 1), dtype=np.uint64)
    final_S = np.zeros((state_space_size, 1), dtype=np.uint64)
    for initial_state in State.get_initial_states():
        initial_state_hash = initial_state.get_hash()
        S[initial_state_hash, 0] = 1
    final_S += S
    for n_digits in range(2, N+1):
        S = np.matmul(T, S)
        S = np.mod(S, MOD)
        final_S += S
        final_S = np.mod(final_S, MOD)
    ans = (10**N-1) * (10**N-2) // 2
    for state1_hash in range(2, state_space_size):
        for state2_hash in range(state1_hash+1, state_space_size):
            if state1_hash & state2_hash == 0:
                ans -= (final_S[state1_hash, 0] * final_S[state2_hash, 0])
    print(ans % MOD)
