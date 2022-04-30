# Question: https://projecteuler.net/problem=539

"""
    - From https://oeis.org/A347325, we have that, a(n) = 2 * (n//2) + 2 - 2*a(n//2)
    - Let s(k) denote sum(from a(2**(k-1)) to a(2**k - 1))
    - e.g., s(1) = a(1)
            s(2) = a(2) + a(3)
            s(3) = a(4) + a(5) + a(6) + a(7)
            ...
    - We have, s(k) has 2**(k-1) terms
           and s(k) = 2 * sum((j//2) for j in [2**(k-1) .. 2**k-1]) + 2*(2**(k-1)) - 4 * sum(a(j//2) for j in [2**(k-1) .. 2**k-1])
                    = alpha                                         + beta         - gamma

      . alpha = 2 * (2**(k-2) + 2**(k-2) + (2**(k-2) + 1) + (2**(k-2) + 1) + ... + (2**(k-1) - 1) + (2**(k-1) - 1))
              = 4 * (2**(k-2) + (2**(k-2) + 1) + ... + (2**(k-1) - 1))
              = 4 * (2**(k-2) * 2**(k-2) + sum([0 .. 2**(k-2)-1]))
              = 4 * (2**(2k-4) + 2**(k-2) * [2**(k-2) - 1] // 2)
              = 4 * (2**(2k-4) + 2**(2k-5) - 2**(k-3))
      . beta = 2**k
      . gamma = 4 * s(k-1)
      . So, s(k) = 4 * (2**(2k-4) + 2**(2k-5) - 2**(k-3)) + 2**k - 4 * s(k-1) for k > 2
    - Let START = 2**(k_1) + 2**(k_2) + .. + 2**(k_m) where (k_1 > k_2 > ... > k_m)
          END   = START + 2**(k_(m+1)) where k_m > k_(m+1)
      Let g(START, END) = sum(from a(START) to a(END-1))
      Let STEP = 2**(k_(m+1)) # in other words, STEP is the number of terms in g(START, END)
      We have, g(START, END) = a(START) + a(START+1) + ... + a(START+STEP-1)
                             = 2*(START//2) + 2 - 2*a(START//2) + 2 * ((START+1)//2) + 2 - 2*a((START+1)//2) + ...
                             = 2*sum([START//2 .. (START+STEP-1)//2]) + 2 * STEP - 2 * sum(from a(START//2) to a((START+STEP-1)//2))
                             = 2*alpha                                + 2 * beta - 2 * gamma
      . alpha = ... = STEP * (START//2) + (STEP // 2) * (STEP // 2 - 1) // similar to alpha in s(k)
      . beta = STEP
      . gamma = g(START//2, START//2 + STEP//2 -1) // this is possible because START is divisible by 2**(k_m), STEP is divisible by 2**(k_(m+1))
      . So, g(START, END) = STEP * (START//2) + (STEP // 2) * (STEP // 2 - 1) + STEP + g(START//2, START//2 + STEP//2 -1)
                          = ... (recursively compute g)
      . Note that, g(START, START) = a(START), which can be computed recursively.
                   g(START, START) is guaranteed to happen after k_(m+) recursive steps.
"""

from math import log2

N = 10**18
MOD = 987654321

cache_p = {1: 1}
def evaluate_P_n(n):
    if not n in cache_p:
        cache_p[n] = 2 * (n//2 + 1 - evaluate_P_n(n//2))
    return cache_p[n]

def evaluate_P_range(begin, end):
    step = end - begin + 1
    assert(2**int(log2(step)) == step)
    assert(begin % 2 == 0)

    if step == 1:
        return evaluate_P_n(begin)

    sum = 0

    sum += step * (begin // 2) + (step // 2) * (step // 2 - 1)
    sum += step
    sum -= 2*evaluate_P_range(begin//2, begin//2 + step//2-1)

    sum *= 2

    return sum

def evaluate_P_range_1_pow_2_n(n): # begin = 1, end = 2**n-1
    sum = 5

    s = 0
    prev_s = 4

    for k in range(3, n+1):
        s = 4 * (2**(2*k-4) + 2**(2*k-5) - 2**(k-3)) + 2**k - 4*prev_s
        sum += s
        prev_s = s

    return sum

if __name__ == "__main__":
    offset = 0
    pow2 = 1
    ans = 0
    exp = 1
    while 2**(exp+1) < N:
        exp += 1
    ans += evaluate_P_range_1_pow_2_n(exp)
    offset = 2**exp
    while offset < N:
        while 2*pow2 + offset <= N:
            pow2 *= 2
        ans += evaluate_P_range(offset, offset+pow2-1)
        offset = offset + pow2
        pow2 = 1
   
    if offset == N:
        ans += evaluate_P_n(N)

    print(ans % MOD)
