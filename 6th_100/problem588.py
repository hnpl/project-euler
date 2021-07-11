# Question: https://projecteuler.net/problem=588

# https://oeis.org/A247649

"""
    Let f(x) = x^4 + x^3 + x^2 + x + 1
    Then Q(N) = (f(x) ** N) (mod 2)
    Some notes:
        * We can do polynomial multiplication via np.convolve, as np.convolve(a,v,'full') is essentially
    polynomial multiplication.
        * Using a method similar to "exponentiation by squaring" (https://en.wikipedia.org/wiki/Modular_exponentiation#Right-to-left_binary_method), we can calculate Q(N) in O(log(N)) rather than in O(N).
        * From the FORMULA section in https://oeis.org/A247649, we can quickly find Q(n) as followed,
            + In the binary form of n, split the binary form by '00'. For example, split('01100010') = ('11', '10').
            + Let (a_1, a_2, ..., a_m) be the splitted form, then
                Q(N) = Q(a_1) * Q(a_2) * ... * Q(a_m)
            + For n that couldn't be furtherly splitted into more than 1 element, we have to manually calculate Q(n).
            + Using the relation, we "only" need to use polynomial multiplication to calculate Q(n) of n upto around 27000.
"""

import numpy as np

N = 18

def Q_brute_force(n):
    f = np.array([1])
    g = np.array([1,1,1,1,1])
    while n > 0:
        bit = n % 2
        if bit == 1:
            f = np.convolve(f, g)
        g = np.convolve(g, g)
        n //= 2
    return np.sum(f%2)

""" # This is also correct
def Q_brute_force(n):
    f = np.array([1,1,1,1,1])
    g = np.array([1,1,1,1,1])
    for _ in range(n-1):
        f = np.convolve(f, g)
    return np.sum(f%2)
"""

def Q(n, cache):
    binary_form_n = bin(n)[2:]
    parts = binary_form_n.split('00')
    ans = 1
    if len(parts) == 1:
        return Q_brute_force(int(parts[0], 2))
    for part in parts:
        if not part:
            continue
        next_n = int(part, 2)
        if not next_n in cache:
            cache[next_n] = Q(next_n, cache)
        ans *= cache[next_n]
    return ans

if __name__ == "__main__":
    ans = 0
    cache = {0:1, 1:5}
    for exp in range(1, N+1):
        n = 10**exp
        ans += Q(n, cache)
    print(ans)
