# Question: https://projecteuler.net/problem=207

"""
    . We have that 4^t, 2^t and k are integers.
      Since k = 4^t - 2^t, the variable t must be of the form t = log(n)/log(2) where n is an integer.
      Since t is of the above form, t is an integer when n is a power of 2.
      For each n, we have one value of k.
    . First few values of n, t, k,
       n   |       t       |   k
    -------+---------------+-------
       2   |       1       |   2
       3   |  non-integer  |   6
       4   |       2       |  12
       5   |  non-integer  |  20
       6   |  non-integer  |  30
     So, the ratio is P(n) = floor(log2(n)) / (n-1) # note that we start counting n from 2
     Note that, between n = 2**x and n = (2**(x+1) - 1), the number of perfect partitions is x, so, the P(n) is monotonically descreasing within the range.
    . Algorithm:
        . We know that for each x (defined as above), we know the range of the image of P(2**x) and P(2**(x+1)-1).
          So, we iterate each integer x until we find the one whose the range of the image contains 1/12345.
        . For that specific x, P(n) is monotonically descreasing within the range where n \in [2**x, 2**(x+1)-1].
          So, we can perform binary search to find the first n such that P(n) < 1/12345.
"""

from math import log

def k(n):
    t = log(n) / log(2)
    return 4**t - 2**t

def binary_search(lowerbound, upperbound, target, exp):
    L = lowerbound
    R = upperbound

    while (L < R):
        M = (L+R)//2
        ratio = exp / (M-1)
        if ratio < target:
            R = M
        else:
            L = M + 1
        print(L, R)
    assert(exp / R < target)
    return R

if __name__ == "__main__":
    ratio = 1.0
    threshold = 1/12345
    exp = 1
    while ratio > threshold:
        exp += 1
        ratio = exp / (2**exp-1)
        print(exp, "[", ratio, "->", exp/(2**(exp+1)-2), "]", threshold)
    n = binary_search(2**(exp-1), 2**exp, threshold, exp-1)
    print(k(n))