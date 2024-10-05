# Question: https://projecteuler.net/problem=698

"""
  . Use multinomial coeffs to find how many digits are there in the answer.
  . Knowing how many digits are there in the answer, we build the answer from the higher place values to the lower place values.
    . At each place value, we have 3 candidates: 1, 2, 3.
    . Define "prefix" as digits of the place values that we built.
    . We can try adding each candidate to the existing prefix, then count how many 123-numbers with new prefix are there that are smaller and equal to the highest 123-number with new prefix (i.e., the number of 123-numbers with new prefix).
        . new_prefix = prefix + 1_of_the_candidates
    . We iterate through the candidates {1,2,3} in that order, and choose the first number such that it is larger than or equal to n_remaining.
       . n_remaining is defined such that, (N - n_remaining) is the number of 123-numbers that are smaller than the smallest number with the existing prefix.
"""

from math import factorial

N = int("".join(["111", "111", "111", "111", "222", "333"]))
#N = 6000
MOD = int("".join(["123"] * 3))

element_list = [0, 1, 2, 3, 11, 12, 13, 21, 22, 23, 31, 32, 33, 111, 112, 113, 121, 122, 123, 131, 132, 133, 211, 212, 213, 221, 222, 223, 231, 232, 233, 311, 312, 313, 321, 322, 323, 331, 332, 333]
element_set = {element: idx for idx, element in enumerate(element_list)}

def ordered_multi_set(elements):
    n = sum(elements)
    ans = factorial(n)
    for element in elements:
        ans //= factorial(element)
    s = set(elements)
    n_distinct_elements = len(s) # 1->1, 2->3, 3->6
    if n_distinct_elements == 1:
        ans *= 1
    elif n_distinct_elements == 2:
        ans *= 3
    elif n_distinct_elements == 3:
        ans *= 6
    return ans

def multi_set(elements):
    n = sum(elements)
    ans = factorial(n)
    for element in elements:
        ans //= factorial(element)
    return ans

def dfs3(depth, max_depth, path, prev_sum, max_sum, element_set, element_list):
    if depth == max_depth-1:
        last_element = max_sum - prev_sum
        if not last_element in element_set:
            return 0
        if element_list[path[-2]] > last_element:
            return 0
        path[-1] = element_set[last_element]
        elements = []
        for idx in path:
            elements.append(element_list[idx])
        return ordered_multi_set(elements)

    ans = 0

    prev_element_idx = 0
    if not depth == 0:
        prev_element_idx = path[depth-1]

    for curr_element_idx in range(prev_element_idx, len(element_list)):
        path[depth] = curr_element_idx
        curr_element = element_list[curr_element_idx]
        next_sum = prev_sum + curr_element
        if next_sum > max_sum:
            break
        ans += dfs3(depth+1, max_depth, path, next_sum, max_sum, element_set, element_list)
    return ans

def dfs2(depth, max_depth, path, prev_sum, max_sum, element_set, element_list, existing_elements): # pre-conditioned
    if depth == max_depth-1:
        last_element = max_sum - prev_sum
        if not last_element in element_set:
            return 0
        if last_element < existing_elements[-1]:
            return 0
        path[-1] = element_set[last_element]
        elements = []
        for i, idx in enumerate(path):
            elements.append(element_list[idx] - existing_elements[i])
        print(elements, multi_set(elements))
        return multi_set(elements)
    ans = 0
    for curr_element_idx in range(0, len(element_list)):
        path[depth] = curr_element_idx
        curr_element = element_list[curr_element_idx]
        if curr_element < existing_elements[depth]:
            continue
        next_sum = prev_sum + curr_element
        if next_sum > max_sum:
            break
        ans += dfs2(depth+1, max_depth, path, next_sum, max_sum, element_set, element_list, existing_elements)
    return ans

def dfs(depth, max_depth, path, prev_sum, max_sum, partitions):
    if depth == max_depth-1:
        path[-1] = max_sum - prev_sum
        partitions.append(path[:])
        return
    for i in range(max_sum - prev_sum + 1):
        path[depth] = i
        dfs(depth+1, max_depth, path, prev_sum + i, max_sum, partitions)

def get_partitions(n_partitions, partition_sum):
    partitions = []
    dfs(0, n_partitions, [0] * n_partitions, 0, partition_sum, partitions)
    return partitions

def count_123_numbers(n_digits):
    ans = dfs3(0, 3, [0]*3, 0, n_digits, element_set, element_list)
    return ans

def solve():
    ans = 0
    counted_numbers = 0
    n_digits = 0 # the result 123-number will have this many digits
    while counted_numbers < N:
        n_digits += 1
        counted_numbers += count_123_numbers(n_digits)

    dp = [[[0 for i in range(n_digits+1)] for j in range(n_digits+1)] for k in range(n_digits+1)]
    dp[0][0][0] = 1
    for i in range(n_digits+1):
        for j in range(n_digits+1):
            for k in range(n_digits+1):
                if i >= 1:
                    dp[i][j][k] += dp[i-1][j][k]
                if j >= 1:
                    dp[i][j][k] += dp[i][j-1][k]
                if k >= 1:
                    dp[i][j][k] += dp[i][j][k-1]
    
    # backtracking
    digits = []
    digit_freq = {1: 0, 2: 0, 3: 0}
    n_remaining = N
    for i in range(1, n_digits):
        n_remaining -= count_123_numbers(i)
    for n_remaining_digits in range(n_digits, 0, -1):
        candidates = {1: 0, 2: 0, 3: 0}
        for next_digit in [1,2,3]:
            digit_freq[next_digit] += 1
            existing_elements = [digit_freq[1], digit_freq[2], digit_freq[3]]
            candidates[next_digit] = dfs2(0, 3, [0]*3, 0, n_digits, element_set, element_list, existing_elements)
            digit_freq[next_digit] -= 1
        print(candidates, n_remaining, existing_elements)
        if candidates[1] >= n_remaining:
            digits.append(1)
        elif candidates[1] + candidates[2] >= n_remaining:
            digits.append(2)
            n_remaining -= candidates[1]
        elif candidates[1] + candidates[2] + candidates[3] >= n_remaining:
            digits.append(3)
            n_remaining -= candidates[1] + candidates[2]
        else:
            assert(False)
        digit_freq[digits[-1]] += 1
    ans = digits

    return ans

if __name__ == "__main__":
    ans = solve()
    ans = "".join(list(map(str, ans)))
    ans = int(ans, 10)
    ans %= MOD
    print(ans)
