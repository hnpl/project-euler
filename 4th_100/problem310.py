# Question: https://projecteuler.net/problem=310

# https://en.wikipedia.org/wiki/Subtract_a_square#Extensions

from math import sqrt
from collections import Counter

N = 100000

isqrt = lambda i: int(sqrt(i))

# https://oeis.org/A014586
def A014586_list(n):
    res = []
    for i in range(n+1) :
        moves = list({res[i-r**2] for r in range(1, isqrt(i)+1)})
        moves.sort()
        k = len(moves)
        mex = next((j for j in range(k) if moves[j] != j), k)
        res.append(mex)
    return res

nim_map = A014586_list(N)
freq = Counter(nim_map)


count = 0

keys = sorted(list(freq.keys()))

# v_a <= v_b <= v_c
for i, v_a in enumerate(keys):
    f_a = freq[v_a]
    for v_b in keys[i:]:
        f_b = freq[v_b]
        v_c = v_a ^ v_b
        if v_c in freq and v_c >= v_b:
            #count = count + f_a * f_b * freq[v_c]
            f_c = freq[v_c]
            if v_a == v_b and v_b == v_c:
                count = count + f_a * (f_a+1) * (f_a+2) // 6
            elif v_a == v_b:
                count = count + f_a * (f_a + 1) // 2 * f_c
            elif v_b == v_c:
                count = count + f_b * (f_b + 1) // 2 * f_a
            elif v_a == v_c:
                count = count + f_c * (f_c + 1) // 2 * f_b
            else:
                count = count + f_a * f_b * f_c

print(count)

