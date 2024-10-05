# Question: https://projecteuler.net/problem=156

"""
  . Calculating how many times do the digit d appear in all numbers in the range [1..N]:
    . From problem 778,
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

  . For finding f(n, d) == n, we build n digit-by-digit. We iterate all possible digits (0 to 9) starting from the highest place value.
	. Note that f(n, d) is monotonically increasing as n increases (1).
    . At place value k, suppose that we built the digits at the higher place values higher than k or equals to k, then suppose we have the number `curr` (curr = abc...e00000...0, where a,b,c,...,e are built digits),
		. The base level is the where place_value = 0. Here, we check if curr == f(curr, d).
		. At other place values, we consider all cases in the range [curr, curr+10**place_value] (i.e., [abc...e000...0, abc..e999...9])
			. We can prune the range if,
				. a. The start of the range is bigger than f(range_end, d) (i.e., range_start > f(range_end, d)).
					. We can prune this range because, due to (1), we always have f(range_end, d) > f(range_start, d), which means range_start > f(range_start, d), so it is not possible to have f(j, d) = j for all j in the range [curr, curr + 10**place_value].
				. b. The end of the range is smaller than f(range_start, d) (i.e., range_end < f(range_start, d)).
					. We can prune this range because, due to (1), we always have f(range_start, d) < f(range_end, d), which means f(range_end, d) < range_end, so it is not possible to have f(j, d) = j for all j in the range [curr, curr + 10**place_value].
		. Then, in the next level of the recursion, we consider all possibilities for the next place value at k-1, e.g., next = abc...ef0000...0, where f is the new digit (i.e., we consider all cases where f = 0 to f = 9).
"""

from math import log, ceil

# From problem 778
def count_digit_at_place_value_of_a_range(place_value, digit, n):
    ans = 0

    segment_offset = digit * (10 ** place_value)
    segment_length = 10 ** place_value
    segment_distance = 10 ** (place_value + 1)

    n_whole_segments = (n - segment_offset) // segment_distance
    cutoff_segment_legnth = (n - segment_offset) % segment_distance + 1

    ans = n_whole_segments * segment_length + min(cutoff_segment_legnth, segment_length)

    return ans

def f(n, d): # number of digit d in the range [0..n]
    n_digits = len(str(n))
    ans = 0
    for place_value in range(n_digits):
        ans += count_digit_at_place_value_of_a_range(place_value, d, n)
    return ans

def digit_base_search(digit, curr, place_value, ans):
    range_size = 10**place_value
    range_start = curr
    range_end = curr + range_size

    f_range_start = f(range_start, digit)

    if place_value == 0:
        if range_start == f_range_start:
            ans[0] += range_start
        return

    f_range_end = f(range_end, digit)

    if f_range_start > range_end or f_range_end < range_start:
        return

    for digit_at_place_value in range(10):
        digit_base_search(digit, curr + digit_at_place_value * (10**(place_value-1)), place_value-1, ans)

def s(d): # find n such that f(n, d) == n for a particular d
    ans = [0]
    digit_base_search(d, 0, 18, ans)
    return ans[0]

if __name__ == "__main__":
    ans = 0
    for digit in range(1, 10):
        ans += s(digit)
    print(ans)
