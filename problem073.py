# Question: https://projecteuler.net/problem=73

# F(n) -> length of Farey Sequence (https://mathworld.wolfram.com/FareySequence.html)

N = 12000


# https://en.wikipedia.org/wiki/Farey_sequence#Next_term
# From 0/1 to end_n/end_d
def farey_sequence_length(end_n, end_d, n):
    a, b, c, d = 0, 1, 1, n
    ans = 1
    while not (a == end_n and b == end_d):
        x = (n + b) // d
        a, b, c, d = c, d, x * c - a, x * d - b
        ans = ans + 1
    return ans

print(farey_sequence_length(1,2,12000)-farey_sequence_length(1,3,12000) - 1)

