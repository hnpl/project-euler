# Problem: https://projecteuler.net/problem=401

"""
    Let S = {1 .. N}.
    Every n has a divisor of 1.                                      -> S has N numbers having a divisor of 1.    -> N * 1^2
    Every other n has a divisor of 2.                                -> S has N/2 numbers having a divisor of 2.  -> floor(N/2) * 2^2
    Of every three consecutive n, one of them has a divisor of 3.    -> S has N/3 numbers having a divisors of 3. -> floor(N/3) * 3^2
    Of every four consecutive n, one of them has a divisor of 4.     -> S has N/4 numbers having a divisors of 4. -> floor(N/4) * 4^2
    Of every five consecutive n, one of them has a divisor of 5.     -> S has N/5 numbers having a divisors of 5. -> floor(N/5) * 5^2
    And so on ...


    So, we need to calculate, 
        sum_{i=1 to N}(floor(N/i) * i^2)

    We have N = 10**15, so we cannot iterate through all i: 1 <= i <= N.

    Instead, first, we calculate sum_{i=1 to floor(sqrt(N))}(floor(N/i) * i^2)
             then, we calculate sum_{i=1 to floor(sqrt(N))} (i * sum_square(floor(N/i) to floor(N/(i+1)))
    This is a result of,
        For all j such that floor(N/i) <= j < floor(N/(i+1)), floor(N/j) = i.
    We only iterate upto floor(sqrt(N)) twice, so O(sqrt(N)).
"""

from math import sqrt

N = 10**15
MOD = 10**9

sqrt_N = int(sqrt(N))

ans = 0

N_divides_ = [0] * (sqrt_N+2)
for i in range(1, sqrt_N+1):
    N_divides_[i] = N // i
    ans = ans + (i**2) * N_divides_[i]
    ans = ans % MOD

N_divides_[sqrt_N+1] =  N // (sqrt_N+1)

sum_power_two = lambda n: n*(n+1)*(2*n+1)//6

for i in range(1, sqrt_N+1):
    ans = ans + i * (sum_power_two(N_divides_[i]) - sum_power_two(max(sqrt_N, N_divides_[i+1])))
    ans = ans % MOD
print(ans)

