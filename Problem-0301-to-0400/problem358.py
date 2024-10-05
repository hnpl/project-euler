# Problem: https://projecteuler.net/problem=358

from sage.all import *

"""
    . According to https://en.wikipedia.org/wiki/Cyclic_number,
        <quote>
            Conversely, if the digital period of 1/p (where p is prime) is p − 1, then the digits represent a cyclic number.
        </quote> (1)
    and,
        <quote>
            From the relation to unit fractions, it can be shown that cyclic numbers are of the form of the Fermat quotient
                (b^(p-1)-1)/p
            where b is the number base (10 for decimal), and p is a prime that does not divide b. (Primes p that give cyclic numbers in base b are called full reptend primes or long primes in base b).
        </quote> (2)

    . Note that, a prime p is a full reptend prime when
        Mod(10, p).multiplicative_order() is p-1 (which means the cycle length is p-1). (3)

    . Let the cyclic number we want to find be 'alpha'.

    . From (1), we can say that, the cyclic number we want to find is in the form of
        alpha = (1/p) * (10**(p-1))

    . Since the left most digits of the cyclic number is 00000000137, then
        0.00000000137 < 1/p < 0.00000000138
        Therefore, floor(1/0.00000000137) > p > floor(1/0.00000000138)
    -> we have a lowerbound and an upperbound for p. (4)

    . We have that the rightmost digits of alpha is 56789.
      From (2), we have that (10^(p-1)-1)/p = alpha.
                       -> 10^(p-1)          = alpha * p + 1
                       -> 10^(p-1) mod 10^5 = (alpha * p + 1) mod 10^5
                       ->                 0 = {[alpha (mod 10^5)] * [p (mod 10^5)] + 1} (mod 10^5)
                       ->                 0 = [56789 * (p mod 10^5) + 1] (mod 10^5) (5)

    . From (2), (3), (4), (5), we can find the prime that produces alpha using the following algorithm,
        for p in [floor(1/0.00000000138), floor(1/0.00000000137)]:
            if [56789 * (p mod 10^5) + 1] (mod 10^5) == 0 and Mod(10, p).multiplicative_order() == p - 1:
                desired_p = p
                break

    . Note that, due to the definition of cyclic number, each rotation must appear once in the set S = {alpha*k | 1 <= k <= desired_p - 1}
      Therefore, for every position i, the i^th digit of alpha must appear as the last digit in one rotation in S.
      (otherwise, by pigeon hole theorem, there would be two distinct k_1 and k_2 in [1, desired_p - 1] such that alpha * k_1 = alpha * k_2, which disagrees with integer arithmetic).

    . Therefore, we can find the sum of the digits by simply summing up the last digits of desired_p * k for all k in [1, desired_p - 1].
      In other words, digits_sum = sum(k in [1, desired_p-1]) [(desired_p *k) mod 10]
                                 = sum(k in [1, desired_p-1]) [(desired_p mod 10) * (k mod 10)]
                                 = (desired_p mod 10) * sum(k in [1, desired_p-1]) [k mod 10]
      Note that (k mod 10) repeatedly cycle around [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]; and 0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 = 45
      So sum(k in [1, desired_p-1]) [k mod 10]
       = sum(k in [0, desired_p-1]) [k mod 10]
       = sum([0..9]) * floor((desired_p-1) / 10) + sum([0..(desired_p-1) mod 10])
       = 45 * floor((desired_p-1) / 10) + sum([0..(desired_p-1) mod 10])
      As a result, digits_sum = (desired_p mod 10) * [45 * floor((desired_p-1) / 10) + sum([0..(desired_p-1) mod 10])]
"""


if __name__ == "__main__":
    ans = 0

    p_lowerbound = int(1/0.00000000138)
    p_upperbound = int(1/0.00000000137)


    RIGHT_MOST_DIGITS = 56789
    MOD = 10**5

    p = p_lowerbound
    while p < p_upperbound:
        m = (RIGHT_MOST_DIGITS * (p % MOD) + 1) % MOD
        if m == 0 and (Mod(10, p).multiplicative_order()) == p-1:
            break
        p = next_prime(p)

    digits_sum = 0
    desired_p = p

    sum_0_n = lambda n: n*(n+1)//2

    digits_sum = (desired_p % 10) * (sum_0_n(9) * ((desired_p-1) // 10) + sum_0_n((desired_p-1) % 10))

    ans = digits_sum

    print(ans)
