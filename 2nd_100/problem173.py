# Question: https://projecteuler.net/problem=173

N = 10**6

# If the hole has odd sides (eg 1x1, 3x3, etc)
#   -> the outer square must have odd sides (eg 3x3, 5x5, etc)
# Similarly, if the hole has even sides (eg 2x2, 4x4, etc)
#   -> the outer square must have even sides (eg 2x2, 4x4, etc)
# The number of tiles of one layer of the outer square is 4*n where n is the side of the outer square
# Since the outer square can have multiple layers, the total number of tiles is 4 * the sum of odd (or even) numbers
# We can fix the number of the first inner layer of the square, supposely i, and find the maximum j such that
#    4 * sum_even([i,j]) <= N

sum_even = lambda x: (x//2)*(x//2 + 1)
sum_odd  = lambda x: x*(x+1)//2 - sum_even(x)
num_of_evens = lambda start, end: ((end//2)*2-start)//2 + 1
num_of_odds = lambda start, end: end - start + 1 - num_of_evens(start, end)

n_even = 0
n_even_1_layer = N // 4 // 2        # layers = even numbers in [2, n_even_1_layer]
j = 0
# sum evens numbers from [i -> j] such that sum_even(j) - sum_even(i) <= N // 4
# so, sum_even(j) <= N // 4 + sum_even(i), where j in [i, N//4]
s = 0
j = 0
for i in range(0, N//4, 2):
    s = s - i
    while s + j + 2 <= N//4:
        s = s + j + 2
        j = j + 2
    n_even = n_even + num_of_evens(i+2, j)

n_odd = 0
n_odd_1_layer = (N // 4 // 2) - 1   # layers = odd numbers in [3, n_odd_1_layer]
j = 1
s = 1
# sum odd numbers from [i -> j] such that sum_odd(j) - sum_odd(i) <= N // 4
# so, sum_odd(j) <= N // 4 + sum_odd(i), where j in [i, N//4]
for i in range(1, N//4, 2):
    s = s - i
    while s + j + 2 <= N//4:
        s = s + j + 2
        j = j + 2
    n_odd = n_odd + num_of_odds(i+2, j)

print(n_odd + n_even)