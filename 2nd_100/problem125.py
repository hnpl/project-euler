# Question: https://projecteuler.net/problem=125

from math import sqrt

N = 10**8
sqrt_N = int(sqrt(N))

def is_palindromic(sum):
    return str(sum) == ''.join(reversed(str(sum)))

sq = [i*i for i in range(sqrt_N+1)]
sum_set = set()
for left in range(1, sqrt_N+1):
    s = sq[left]
    for right in range(left+1, sqrt_N+1):
        s = s + sq[right]
        if s >= N:
            break
        sum_set.add(s)

ans = 0
for sum in sum_set:
    ans = ans + sum * is_palindromic(sum)

print(ans)