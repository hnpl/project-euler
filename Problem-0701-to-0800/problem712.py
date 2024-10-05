# Question: https://projecteuler.net/problem=712

"""
  . v_p(n) != 0 iff p | n.
    . e.g., for v_p(24), we have 24 = 2^3 * 3
      . v_2(24) = 3
      . v_3(24) = 1
      . v_p(24) = 0 for other primes p
  . So, D(n, m) is essentially sum of the differences of the exponents of prime factors of n and m.
    . e.g., for D(14, 24), we have 14 = 2 * 7 and 24 = 2^3 * 3, then,
      . D(14, 24) = sum(| v_p(14) - v_p(24) |) for p in {2, 3, 7}
                  = | v_2(14) - v_2(24) | + | v_3(14) - v_3(24) | + | v_7(14) - v_7(24) |
                  = | 1 - 3 | + | 0 - 1 | + | 1 - 0 |
                  = 4
  . So, we can see S(N) as pairwise exponent differences of prime factors for all prime factors.
    . i.e., if we pick a particular prime number such as p, then we can count how many of the numbers in range [1, N] are divisible upto p^0, upto p^1, upto p^2, ... We can do this in O(log(N)), and we know v_p(k) for all k in [1, N].
    . Then, for a specific p, we can find the sum of | v_p(n) - v_p(m) | for all pair of n,m in the range [1, N]. Denote this sum as g(p).
       g(p) = sum(|i - j| * freq_p(i) * freq_p(j)) for all i, j
        where freq_p(i) is the number of numbers in range [1,N] that are divisible by upto p**i.
  . There are two methods, of calculating this sum,
    . Method 1: we iterate through all primes p and calculate g(p) for each p.
    . Method 2: if all number in the range [1,N] are divisible by upto p**1, we can manually calculate g(p) = freq_p(0) * freq_p(1) = (N - q) * q for a particular q.
      . Note that, for all primes in range [N//(q+1)+1, N//q], they have the same value for g(p) = (N-q)*q.
      . We can also calculate how many primes in the range [N//(q+1)+1, N//q] using prime_pi(N//q) - prime_pi(N//(q+1)).
      . So, we can iterate through all possible q.
  . So, we can calculate the S(N) in O(sqrt(N)) iterations by,
    . Using method 1 for the range [1..sqrt(N)].
    . Using method 2 for the range [sqrt(N)+1, N], which corresponds to q in [1..sqrt(N)].
"""

from sage.all import *

N = 10**12
MOD = 10**9 + 7

def prime_factor_count(n, prime):
    p = prime
    count = []
    count.append(n)
    while p <= n:
        count.append(n // p)
        p *= prime
    for i in range(len(count)-1):
        # currently, count[i] counts the number of numbers in the range [1..N] that is divisible by p**i, p**(i+1), p**(i+2), etc.
        # however, we want the number of numbers divisible up to p**i (i.e., not divisible by p**(i+1))
        # we can calculate this by simply subtracting the number of numbers divisible by p**i, p**(i+1), p**(i+2), etc. by the number of number divisible by p**(i+1), p**(i+2), etc.
        count[i] = count[i] - count[i+1]
    return count

def calculate_pairwise_difference(arr):
    ans = 0
    n = len(arr)
    for i in range(n):
        for j in range(i+1, n):
            ans += (j - i) * arr[j] * arr[i]
            ans %= MOD
    return ans

if __name__ == "__main__":
    ans = 0

    sqrt_N = int(ceil(sqrt(N)))

    prime_generator = primes(sqrt_N)
    for prime in prime_generator:
        ans += calculate_pairwise_difference(prime_factor_count(N, prime))

    for quotient in range(1, floor(sqrt(N))):
        end = N // quotient
        start = N // (quotient + 1) + 1
        #print(quotient, start, end, N / start, N / end)
        n_primes = prime_pi(end) - prime_pi(start-1)
        s = calculate_pairwise_difference([N - quotient, quotient]) * n_primes
        #print(quotient, end, start, s, prime_pi(end), prime_pi(start))
        s %= MOD
        ans += s
        ans %= MOD

    ans *= 2
    ans %= MOD
    print(ans)

