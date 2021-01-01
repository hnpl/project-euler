# Question: https://projecteuler.net/problem=240

"""
    . First, we find all combinations C of 10 numbers that sum to 70.
    . Now we need to count how to many distinct ways to arrange C in order while having to fill out the rest of the numbers, in this case, the bottom 10 numbers.
      There is a problem. The minimum value in the top 10 numbers could also be in the bottom 10 numbers.
    . We know that if we have the histogram of a set of a number, we can count the number of ways to arrange them through multinomial theorem.
    . Let m be the minimum value in C, and let hist(C) be the histogram of C, and let hist(C)[m] be the frequency of m in C.
      We rearrange the `top` set and the `bottom` set by moving all value m in bottom to top (moving from top to bottom might result in overcounting).
      We can do that by:
        For each possible value of m (keep increasing m till C has 20 numbers):
            * We have the modified hist(C) of the top number. Assume C has n numbers.
            * The number of distinct ways of choosing n places from 20 places: binomial(20, n)
            * The number of distinct ways of arranging C of length n using n places: multinomial(n, hist(C).frequencies())
            * There are 20-n numbers in the bottom set, and the maximum value of the bottom set is min(hist(C)) - 1.
              So, the number of distinct ways of constructing the bottom set is: (min(hist(C)) - 1) ^ (20 - n)
        
"""

N = 12
M = 20
N_TOP = 10
TOP_SUM = 70

factorial = [1]
for i in range(1, M+1):
    factorial.append(factorial[-1] * i)

def binomial(n, k):
    return factorial[n] // factorial[k] // factorial[n-k]

def multinomial(n, ks):
    ans = factorial[n]
    for k in ks:
        ans //= factorial[k]
    return ans

def get_histogram(sample):
    histogram = {}
    for i in sample:
        if not i in histogram:
            histogram[i] = 0
        histogram[i] += 1
    return histogram

def count(histogram):
    min_histogram_val = min(histogram.keys())
    min_val_count = histogram[min_histogram_val]
    ans = 0
    n = sum(histogram.values())

    while n <= M:
        histogram[min_histogram_val] = min_val_count
        n = sum(histogram.values())
        # number of ways to place n numbers in M places: binomial(M, n)
        # from a set of n numbers forming a histogram H, number of distinct ways to place those numbers to n places: multinomial(n, H.freq())
        # there are (M-n) places left to fill with the maximum value being (min(H)-1), so there are (min(H)-1) ** (M-n) ways to fill them
        ans += binomial(M, n) * multinomial(n, histogram.values()) * ((min_histogram_val - 1) ** (M-n))
        n += 1
        min_val_count += 1

    return ans

def dfs(depth, path, curr_sum, target_sum, max_depth, max_val):
    if depth == max_depth:
        if curr_sum == TOP_SUM:
            return count(get_histogram(path))
        return 0

    next_max = max_val
    if depth > 0:
        next_max = path[depth-1]
    next_max = min(next_max, target_sum - curr_sum)

    ans = 0
    for next_val in range(1, next_max+1):
        path[depth] = next_val
        ans += dfs(depth+1, path, curr_sum + next_val, target_sum, max_depth, N)
    return ans


if __name__ == "__main__":
    path = [0] * N_TOP
    ans = dfs(0, path, 0, TOP_SUM, N_TOP, N)
    print(ans)