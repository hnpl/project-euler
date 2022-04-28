# Question: https://projecteuler.net/problem=294

"""
- Strategy:
    * Use Markov chain, where state = (digit_sum, number_mod_23).
    * The state is hashed by number_mod_23 * 24 + digit_sum (there are 24 different digit sums, i.e. {0, 1, 2, ..., 22, 23}.
    * Let S[k] = number of numbers statisfying state with the hash of k.
    * State transformation is equivalent to adding a digit to numbers in a state.
    * We can construct the transformation matrix accordingly.
"""

import numpy as np

N = 11**12
MOD = 10**9
K = 23

def state_index(modK, digit_sum):
    return modK * (K+1) + digit_sum

def pow(M, n):
    base = np.array(M)
    k, _ = M.shape
    result = np.eye(k, dtype=object)
    while n > 0:
        print(n)
        if n % 2 == 1:
            result = np.matmul(result, base)
            result = np.mod(result, MOD)
        n //= 2
        base = np.matmul(base, base)
        base = np.mod(base, MOD)
    return result

if __name__ == "__main__":
    m = (K+1)*K
    T = np.zeros((m, m), dtype=object)
    for curr_modK in range(K):
        for curr_digit_sum in range(K+1):
            curr_state_index = state_index(curr_modK, curr_digit_sum)
            for next_digit in range(10):
                next_digit_sum = curr_digit_sum + next_digit
                if next_digit_sum > K:
                    break
                next_modK = ((curr_modK*10)+next_digit) % K
                next_state_index = state_index(next_modK, next_digit_sum)
                T[next_state_index][curr_state_index] += 1

    S = np.zeros((m, 1), dtype=object)
    S[0][0]=1

    S = np.matmul(pow(T,N), S)
    ans = S[state_index(modK=0, digit_sum=K)][0]
    print(ans)


