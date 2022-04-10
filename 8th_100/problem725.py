# Problem: https://projecteuler.net/problem=725

"""
    . Since a digit is at most 9, we can find all multisets of digits > 0 that results in a sum that is less than or equal to 9.
        . For each multiset of sum K, we add K once to the multiset and add zeros P times to the multiset so that the multiset has 2020 elements.
    . See problem 171 on how to use multinomial theorem to calculate the sum of all permutations of a multiset
"""

from collections import defaultdict
from math import factorial, gcd

N = 2020
MOD = 10**16

def dfs(depth, path, max_depth, curr_sum, target_sum):
    if curr_sum == target_sum:
        yield path[:depth]
    if depth == max_depth:
        return
    digit_lowerbound = 1
    if depth > 0:
        digit_lowerbound = path[depth-1]
    for digit in range(digit_lowerbound, 10):
        path[depth] = digit
        yield from dfs(depth+1, path, max_depth, curr_sum + digit, target_sum)

def multinomial(args):
    s = sum(args)
    ans = factorial(s)
    for i in args:
        ans = ans // factorial(i)
    return ans

if __name__ == "__main__":
    ans = 0

    for digit_sum in range(1, 10):
        for sum_combination in dfs(0, [0]*10, 10, 0, digit_sum):
            combination = defaultdict(lambda: 0)
            n_zeros = N
            combination[digit_sum] = combination[digit_sum] + 1
            n_zeros = n_zeros - 1
            for i in sum_combination:
                combination[i] = combination[i] + 1
                n_zeros = n_zeros - 1
            if n_zeros > 0:
                combination[0] = n_zeros

            n = sum(list(combination.values()))
            if n > N:
                print("rejected ", dict(combination), n)
                continue

            n_permutations = multinomial(list(combination.values()))

            GCD = None
            for digit, digit_freq in combination.items():
                if not GCD:
                    GCD = digit_freq
                else:
                    GCD = gcd(GCD, digit_freq)

            ratio = {}
            ratio_sum = 0
            for digit, digit_freq in combination.items():
                ratio[digit] = digit_freq // GCD
                ratio_sum = ratio_sum + ratio[digit]
            for digit in ratio.keys():
                ratio[digit] = ratio[digit] * n_permutations // ratio_sum

            column_sum = 0
            for digit, digit_freq in ratio.items():
                column_sum += digit_freq * digit

            column_sum = column_sum % MOD
            sum_mod = 0
            coeff = 1
            for i in range(N):
                sum_mod = sum_mod + column_sum * coeff
                sum_mod = sum_mod % MOD
                coeff = coeff * 10

            ans = ans + sum_mod
            ans = ans % MOD

        
    print(ans)