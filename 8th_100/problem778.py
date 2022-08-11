# Question: https://projecteuler.net/problem=778

"""
  . Let c = a [x] b where a = (... a_2, a_1, a_0), b = (... b_2, b_1, b_0), c = (... c_2, c_1, c_0).
                    where c_k = (a_k * b_k) % 10.

  . We want to find F(R, M), which is the sum of all G(x_1, x_2, ..., x_R) where 0 <= x_k <= M for all k
                             and G(x_1, x_2, ..., x_R) = x_1 [x] x_2 [x] ... [x] x_R.

  . Note that, for each G(x_1, x_2, ..., x_R), we can calculate each digit of G independently.
    This means, if we know the frequencies of digits from 0 to 9 at each place value (ones, tens, hundreds ...), we can find the sum of that place value in F using Markov process.
    (The independence among place values results in the property that, for example, we don't need to care if "the digit is 7aaaaa then the next place value must be <= 6")
  . We can set up the Markov process such that, for a fixed place value k,
    . The state S at iteration P is where S[d] means how many tuples of length P having the product of digits at the place value k ends with d.
    . The transition matrix T where T[i,j] = q means, if the product of length P has the ending digit of j, then it will contribute to the product of length P+1 having i as the ending digit q times.
    . We can calculate q by, for all s such that (j * s) % 10 = i, we sum how many times s appears in the place value k across all integers in the range [0, M].

  . The remaining question is, given a range [0, M], how many times s appears at the place value k across all integers in the range [0, M].
    . The first occurance of digit s at the place value k is at s * (10 ** k).
      . E.g., suppose the range is [0, 765], the first occurrence of 9 at tens is 90 = 9 * (10**1).
    . Every time we see the apparence of digit s at the place value k, we'll see them in a cluster of length 10 ** k,
      and the distance between two nearest clusters is 10 ** (k+1).
      . E.g., suppose the range is [0, 765], the occurrences of 9 at tens are [90, 99] (length 10 = 10**1)
                                                                              [190, 199] (length 10) (distance to [90, 99] is 100)
                                                                              [290, 299] (length 10) (distance to [190, 199] is 100)
                                                                              ...
    . So, essentially, we are measuring the length of the considered segments in the range [0,M], which is basically,
      length_of_one_segment * number_of_whole_segments + length_of_tail_segment
"""

import numpy as np
from math import log, ceil

R = 234567
M = 765432
MOD = 10**9+9
N_DIGITS = int(ceil(log(M)/log(10)))

def get_index(place_value, digit):
    return place_value * 10 + digit

def count_digit_at_place_value_of_a_range(place_value, digit, n):
    ans = 0

    segment_offset = digit * (10 ** place_value)
    segment_length = 10 ** place_value
    segment_distance = 10 ** (place_value + 1)

    n_whole_segments = (n - segment_offset) // segment_distance
    cutoff_segment_legnth = (n - segment_offset) % segment_distance + 1

    ans = n_whole_segments * segment_length + min(cutoff_segment_legnth, segment_length)

    return ans

# Method 1: calculating all place values using only one matrix
#def get_T():
#    n = 10 * N_DIGITS
#
#    T = np.zeros((n, n), dtype=np.uint64)
#
#    for place_value in range(N_DIGITS):
#        for old_prod_place_value_digit in range(10):
#            for next_digit in range(10):
#                old_prod_digit_index = get_index(place_value, old_prod_place_value_digit)
#                new_prod_place_value_digit = (old_prod_place_value_digit * next_digit) % 10
#                #for i in range(M+1):
#                #    k = (i // (10**place_value)) % 10
#                #    if k == next_digit:
#                #        next_digit_freq += 1
#                #print(place_value, next_digit, next_digit_freq, count_digit_at_place_value_of_a_range(place_value, next_digit, M))
#                #assert(next_digit_freq, count_digit_at_place_value_of_a_range(place_value, next_digit, M))
#                next_digit_freq = count_digit_at_place_value_of_a_range(place_value, next_digit, M)
#                new_prod_digit_index = get_index(place_value, new_prod_place_value_digit)
#                T[new_prod_digit_index, old_prod_digit_index] += next_digit_freq
#    return T
#
#def get_initial_S():
#    n = 10 * N_DIGITS
#    S = np.zeros((n, 1), dtype = np.uint64)
#    for place_value in range(N_DIGITS):
#        S[get_index(place_value, 1), 0] = 1
#    return S

# Method 2: since the place values can be calculated independently, we generate one matrix per place value
def get_T(place_value):
    n = 10
    T = np.zeros((n, n), dtype = np.uint64)
    for old_prod_place_value_digit in range(10):
        for next_digit in range(10):
            new_prod_place_value_digit = (old_prod_place_value_digit * next_digit) % 10
            next_digit_freq = count_digit_at_place_value_of_a_range(place_value, next_digit, M)
            T[new_prod_place_value_digit, old_prod_place_value_digit] += next_digit_freq
    return T

def get_initial_S():
    n = 10
    S = np.zeros((n, 1), dtype = np.uint64)
    S[1, 0] = 1
    return S


def pow_mod(a, exp, mod):
    n, _ = a.shape

    base = np.array(a)
    ans = np.eye(n, dtype = np.uint64)

    while exp > 0:
        if exp % 2 == 1:
            ans = np.matmul(ans, base)
            ans = np.mod(ans, mod)
        exp //= 2
        base = np.matmul(base, base)
        base = np.mod(base, mod)

    return ans

if __name__ == "__main__":
    ans = 0

# Method 1: calculating all place values using one matrix
# Calculating S = (T**R) * S
#    T = get_T()
#    S = get_initial_S()
#    T_exp = pow_mod(T, R, MOD)
#    S = np.matmul(T_exp, S)
#    S = np.mod(S, MOD)
#    for place_value in range(N_DIGITS):
#        coeff = 10 ** place_value
#        for digit in range(10):
#            ans += coeff * digit * S[get_index(place_value, index), 0]

# Method 2: using one matrix per place value
    for place_value in range(N_DIGITS):
        T = get_T(place_value)
        S = get_initial_S()

        T_exp = pow_mod(T, R, MOD)
        S = np.matmul(T_exp, S)
        S = np.mod(S, MOD)

        coeff = 10 ** place_value
        for digit in range(10):
            ans += coeff * digit * S[digit, 0]

    ans %= MOD

    print(ans)

