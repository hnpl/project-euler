# Question: https://projecteuler.net/problem=706

"""
    * Substring is a subsequence of contiguous characters of a string.
    * The value of N mod 3 equals to (the sum of digits of N) mod 3.
    * Since F(2) = 30, we do not count the UNIQUE substrings, but rather, we are considering non-unique substrings that are divisible by 3.
    * We will use Markov Process (as opposed to Dynamic Progamming) to count the number of substrings that are divisible by 3,
        - Using Markov Process allows us to scale up the problem for large values of N.
        - The state is (k0, k1, k2, prev_k0, prev_k1, prev_k2),
            + k0 is (the number of substrings, which end at the last digit from the left, that are divisible by 3) mod 3.
            + k1 is (the number of substrings, which end at the last digit from the left, that have the remainder of 1 when divided by 3) mod 3.
            + k2 is (the number of substrings, which end at the last digit from the left, that have the remainder of 2 when divided by 3) mod 3.
            + prev_k0 is (the number of substrings, which do not end at the last digit from the left, that are divisible by 3) mod 3.
            + prev_k1 is (the number of substrings, which do not end at the last digit from the left, that have the remainder of 1 when divided by 3) mod 3.
            + prev_k2 is (the number of substrings, which do not end at the last digit from the left, that have the remainder of 2 when divided by 3) mod 3.
        - So, for example, for the number 123, there are 6 substrings, {1, 2, 3, 12, 23, 123}.
            + The substrings {3, 123} will be counted by k0.
            + The substring  {23} will be counted by k2.
            + The substring  {1, 12} will be counted by prev_k0.
            + The substring  {2} will be counted by prev_k2.
            + So, the number 123 is described by the state (2, 0, 1, 2, 0, 1)
        - We will count how many numbers correspond to each possible state.
        - The transistion matrix, T, describes how each state transforms when each of the digits from 0 to 9 are appended to each state.
            + For example, for every number K in the state (2, 0, 1, 2, 0, 1), say, 123,
                . When (a digit mod 3) == 1, i.e. {1, 4, 7}, appended to K,
                    . For each substring, there are two options: we can choose to append the digit to K, or not to append the digit to K.
                        . If we append the digit to K, it will be counted by one of {k0, k1, k2}.
                          Note that, due to the definition of substring, we can only append the new digit to the substrings ending at the last digit from the left.
                        . If we do not append the digit to K, it will be counted by {prev_k0, prev_k1, prev_k2}.
                    . For example, if we add the digit 7 to each of the substrings of 123, the new substrings are,
                        empty -> 7
                        3 -> 37
                        123 -> 1237
                        23 -> 237
                        . So, all of substrings that were counted by k0, will now be counter by k1;
                              all of substrings that were counted by k1, will now be counted by k2;
                              all of substrings that were counted by k2, will now be counted by k0;
                              note that we also add one more substring to k1 (the substring "7").
                        . Also, note that we still have the old substrings {1, 2, 3, 12, 23, 123} but all of them are no longer ended at the last digit from the left.
                        . So, for this case, when we append 7, (or any digit having mod 3 == 1), we transfrom the state (k0, k1, k2, prev_k0, prev_k1, prev_k2) to (k2, (k0+1)%3, k1, (k0+prev_k0)%3, (k1+prev_k1)%3, (k2+prev_k2)%3)).
                . Similarly, we can append (a digit mod 3) == 0, i.e. {0, 3, 6, 9} to K, or (a digit mod 3) == 2, i.e. {2, 5, 8} to K.
        - Then, we will use S * (T**(N-1)) to calculate the number of numbers at each state after appending N-1 digits.
"""

import numpy as np

N = 10**5
MOD = 10**9 + 7

def state_to_index(k0, k1, k2, prev_k0, prev_k1, prev_k2):
    state = 0
    for val, power in zip([k0, k1, k2, prev_k0, prev_k1, prev_k2], range(6)):
        state += val * (3**power)
    return state

def index_to_state(index):
    state = [0, 0, 0, 0, 0, 0]
    i = 0
    while index > 0:
        state[i] = index % 3
        index //= 3
        i += 1
    return state

def F(n):
    n_states = 3**6
    S = np.zeros((n_states, 1), dtype = np.uint64)
    S[state_to_index(1, 0, 0, 0, 0, 0)] = 3
    S[state_to_index(0, 1, 0, 0, 0, 0)] = 3
    S[state_to_index(0, 0, 1, 0, 0, 0)] = 3
    T = np.zeros((n_states, n_states), dtype=np.uint64)
    result_indices = set()
    for k0 in range(3):
        for k1 in range(3):
            for k2 in range(3):
                for prev_k0 in range(3):
                    for prev_k1 in range(3):
                        for prev_k2 in range(3):
                            curr_index = state_to_index(k0, k1, k2, prev_k0, prev_k1, prev_k2)
                            if (k0+prev_k0)%3 == 0:
                                result_indices.add(curr_index)
                            state = [k0, k1, k2, prev_k0, prev_k1, prev_k2]
                            # Adding {0, 3, 6, 9}
                            new_index = state_to_index((k0+1)%3, k1, k2, (k0+prev_k0)%3, (k1+prev_k1)%3, (k2+prev_k2)%3)
                            T[new_index, curr_index] += 4
                            # Adding {1, 4, 7}
                            new_index = state_to_index(k2, (k0+1)%3, k1, (k0+prev_k0)%3, (k1+prev_k1)%3, (k2+prev_k2)%3)
                            T[new_index, curr_index] += 3
                            # Adding {2, 5, 8}
                            new_index = state_to_index(k1, k2, (k0+1)%3, (k0+prev_k0)%3, (k1+prev_k1)%3, (k2+prev_k2)%3)
                            T[new_index, curr_index] += 3

    ans = 0
    exp = n-1
    T_exp = np.eye(n_states, dtype=np.uint64)
    T_base = np.array(T, dtype=np.uint64)
    while exp > 0:
        if exp % 2 == 1:
            S = np.matmul(T_base, S)
            S = np.mod(S, MOD)
        T_base = np.matmul(T_base, T_base)
        T_base = np.mod(T_base, MOD)
        exp //= 2
    for index in result_indices:
        ans += S[index, 0]
    return ans

if __name__ == "__main__":
    ans = F(N)
    print(ans % MOD)
