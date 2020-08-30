# Problem: https://projecteuler.net/problem=201

"""
    . Let state encode the size of subset and the curr_sum of a subset as follows,
        state = curr_sum * (MAX_SUBSET_SIZE + 1) + subset_size
    . This way of encoding states allow us to easily generate the next states.
      For example, if we want to add a new element of value V to the state, we have
      new_state = (curr_sum + V) * (MAX_SUBSET_SIZE + 1) + subset_size + 1
                = curr_sum * (MAX_SUBSET_SIZE + 1) + subset_size + V * (MAX_SUBSET_SIZE+1) + 1
                = state + V * (MAX_SUBSET_SIZE+1) + 1
    . Let DP[i][state] keep the number of combinations of subsets of size j and of sum k where state = k * (MAX_SUBSET_SIZE + 1) + j and the subset consists of elements from the first i elements from the superset.
        . DP[i][state] = 0 if there are no such a combination
        . DP[i][state] = 1 if there's one such a combination
        . DP[i][state] = 2 if there are more than one such a combination
    . We have that,
        DP[i+1][state] = DP[i][state] + DP[i][prev_state]
            where prev_state = (curr_sum - element[i+1]) * (MAX_SUBSET_SIZE + 1) + subset_size - 1
                for each corresponding state where state = curr_sum * (MAX_SUBSET_SIZE + 1) + subset_size
        This is correct as += DP[i][state] means that we decide to not to add the elements[i+1]
                    and    += DP[i][prev_state] means that we decide to add the elements[i+1].
"""

import numpy as np
from scipy.ndimage.interpolation import shift

N = 100
M = 50

elements = [k*k for k in range(1, N+1)]
max_sum = sum(sorted(elements)[-M:])

def encode(subset_size, curr_sum):
    return curr_sum * (M+1) + subset_size
def decode(h):
    return h % (M+1), h // (M+1)

n_states = (M+1) * (max_sum+1)

if __name__ == "__main__":
    ans = 0

    prev_DP = None
    curr_DP = np.zeros(n_states+1, dtype = np.int16)

    curr_DP[encode(0, 0)] = 1

    for i, element in enumerate(elements):
        prev_DP, curr_DP = curr_DP, prev_DP
        curr_DP = prev_DP + shift(prev_DP, element*(M+1) + 1, cval=0)
        #                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        #                   generate the next states from the previous states
        curr_DP[np.where(curr_DP > 1)] = 2

    size_M_index = np.arange(max_sum+1) * (M+1) + M
    result_index = curr_DP[size_M_index]
    ans = np.sum(np.where(result_index == 1))
    print(ans)

