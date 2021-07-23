# Question: https://projecteuler.net/problem=306

# According to https://math.stackexchange.com/questions/615242/game-involving-tiling-a-1-by-n-board-with-1-x-2-tiles, the first values of N where the second player wins are "1, 5, 9, 15, 21, 25, 29, 35, 39, 43, 55, 59, 63, 73, 77, 89, 93, 97, and 99"

# According to https://oeis.org/A215721,
#   If a(n) is an increasing sequence of value k where a 1D strip of length k leads to the second player's win. Then we have, for n > 14, a(n) = a(n-5) + 34

# More info: https://arxiv.org/pdf/math/0612616.pdf, https://arxiv.org/pdf/quant-ph/0311018.pdf

N = 1000000

seq = [1, 5, 9, 15, 21, 25, 29, 35, 39, 43, 55, 59, 63, 73]
next_element = seq[-5] + 34

while next_element <= N:
    seq.append(next_element)
    next_element = seq[-5] + 34

ans = N - len(seq)
print(ans)
