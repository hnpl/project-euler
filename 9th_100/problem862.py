N = 12

factorials = [1]
for i in range(1, N+1):
    factorials.append(factorials[-1] * i)

def path_to_freq(path):
    freq = {}
    for e in path:
        if not e in freq:
            freq[e] = 0
        freq[e] += 1
    return freq

def freq_to_freq_freq(freq):
    freq_freq = {}
    for v in freq.values():
        if not v in freq_freq:
            freq_freq[v] = 0
        freq_freq[v] += 1
    return freq_freq

def freq_to_path(freq):
    path = []
    for x, y in freq.items():
        for i in range(y):
            path.append(x)
    return path

def count_non_zeros(path):
    # num_configurations = number of ways of mapping a digit to an element in the path
    num_configurations = 1
    for i in range(len(path)):
        num_configurations *= (9 - i)
    freq_freq = freq_to_freq_freq(path_to_freq(path))
    #print(path_to_freq(path), freq_freq)
    for k, v in freq_freq.items(): # digits with same number of appearances are symmetrical, those with different #appearances are not
        #print(v)
        num_configurations //= factorials[k] ** v
    num_permutations_per_configuration = factorials[sum(path)]
    for e in path:
        num_permutations_per_configuration //= factorials[e]
    sum_T_all_permutations_per_configuration = num_permutations_per_configuration * (num_permutations_per_configuration - 1) // 2
    ans = sum_T_all_permutations_per_configuration * num_configurations
    #print("non zeros ", path,
    #      num_permutations_per_configuration, num_configurations,
    #      num_permutations_per_configuration * num_configurations)
    return ans

def count_with_zeros(path):
    ans = 0
    freq = path_to_freq(path)
    # what if the digit 0 appears 1 time, or 2 times, or 3 times, and so on
    for num_zero_digits in freq.keys():
        non_zero_path = path[:]
        non_zero_path.remove(num_zero_digits)
        non_zero_freq = path_to_freq(non_zero_path)
        num_configurations = 1
        for i in range(len(non_zero_path)):
            num_configurations *= (9 - i)
        non_zero_freq_freq = freq_to_freq_freq(non_zero_freq)
        for k, v in non_zero_freq_freq.items():
            num_configurations //= factorials[k] ** v
        num_permutations_per_configuration = factorials[sum(path)]
        for e in path:
            num_permutations_per_configuration //= factorials[e]
        removed_one_zero_path = path[:]
        removed_one_zero_path.remove(num_zero_digits)
        if not num_zero_digits == 1:
            removed_one_zero_path.append(num_zero_digits - 1)
        num_permutations_per_configuration_with_leading_zeros = factorials[sum(removed_one_zero_path)]
        for e in removed_one_zero_path:
            num_permutations_per_configuration_with_leading_zeros //= factorials[e]
        num_valid_permutations_per_configuration = num_permutations_per_configuration - num_permutations_per_configuration_with_leading_zeros
        ans += num_valid_permutations_per_configuration * (num_valid_permutations_per_configuration - 1) // 2 * num_configurations
        #print("with zero:", path, num_zero_digits, num_valid_permutations_per_configuration, num_configurations, num_valid_permutations_per_configuration * num_configurations)
    return ans

def dfs(curr_sum, max_sum, path):
    ans = 0
    if len(path) > 10:
        return ans
    if curr_sum == max_sum:
        if len(path) == 10:
            return count_with_zeros(path)
        else:
            return count_non_zeros(path) + count_with_zeros(path)
        #return count_non_zeros(path)
    prev_count = 1
    if len(path) > 0:
        prev_count = path[-1]
    for next_count in range(prev_count, max_sum - curr_sum + 1):
        ans += dfs(curr_sum + next_count, max_sum, path  + [next_count])
    return ans

if __name__ == "__main__":
    ans = dfs(0, N, [])
    print(ans)
