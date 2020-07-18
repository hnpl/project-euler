# Question: https://projecteuler.net/problem=160

"""
Let N = 1000000000000
To find f(N) (mod 10^5), we find f(N) (mod 2^5) and f(N) (mod 5^5) instead.
	1. First, we find f(N) (mod 2^5).
		Find the number of factor of 2 and factor of 5 of N.
		Say they are a and b respectively, i.e. N contains 2^a and 5^b.
 		Obviously, a > b, and the trailing zeros formed by 2^b * 5^b.
 		So f(N) = N / (10^b).
 		f(N) = 2^(a-b) * factors_other_than_2.
 		Quite obviously, a-b > 5, so f(N) (mod 2^5) = 0.
	2. Now, we find f(N) (mod 5^5).
		- We need to find f(N) (mod 5^5),
			f(N) (mod 5^5) = N!/10^b            											  (mod 5^5)
		                   = N! / (2^b * 5^b)   											  (mod 5^5)
			   	           = [(N! / 5^b) (mod 5^5) * inverse_mod(2^b, 5^5)]                   (mod 5^5)
					       = [(N! / 5^b) (mod 5^5) * power_mod(inverse_mod(2, 5^5), b, 5^5)]  (mod 5^5)
		- To find (N! / 5^b) (mod 5^5),
			Let f'(start, end) = product_{all start <= i <= end where i is not divisible by 5} (i).
			Let n = p * 5^5 + q; p = n // 5^5; q = n % 5^5.
			Let g(N) = N! / (all factor 5)
			Then, 
				g(N) = (N! / 5^b) (mod 5^5) 
			         = (product_of_non_multiple_of_5               -----> [f'(1, p * 5^5) (mod 5^5) * f'(p*5^5+1, p*5^5+q)] (mod 5^5)
			           * multiple_of_5_without_factor_5) (mod 5^5) -----> g(p * 5^5 // 5) (mod 5^5) <- this can be done recursively
				                                                   		  # (p*5^5)//5 is the number of multiples of 5 from 1 to p*(5^5)
			+ Observation:
				For an integer k,
					f'(k*(5^5)+1, (k+1)*(5^5)) (mod 5^5) = f'((k+1)*(5^5)+1, (k+2)*(5^5)) (mod 5^5)
				Why? For all i that is not divisible by 5, i (mod 5^5) == (i+5^5) (mod 5^5).
			+ Result:
				f'(1, n) (mod 5^5) = power_mod(f'(1, 5^5) (mod 5^5), p, 5^5) * f'(p*5^5+1, p*5^5+q) (mod 5^5)
				Since q < 5^5, it's feasible to calculate f'(p*5^5+1, p*5^5+q)  (mod 5^5) manually.
		- So, f(N) (mod 5^5) = (g(N) * power_mod(inverse_mod(2, 5^5), b, 5^5)) (mod 5^5).
	3. Now we have, f(N) (mod 2^5) = 0 and let f(N) (mod 5^5) = r.
	gcd(2^5, 5^5) == 1
	We want to find y such that y (mod 2^5) = 0 and y (mod 5^5) = r.
	Using Chinese Remainder Theorem, we can find such y.
	y = crt(0, r, 2^5, 5^5)
"""

from sage.all import *

N = 10**12

count_factor_5 = 0
powers5 = []
p = 5
while p <= N:
    powers5.append(p)
    p = p * 5

for i, power in enumerate(powers5, start=1):
    count_factor_5 = count_factor_5 + N // power

c = 5**5

invmod2 = inverse_mod(2, c)
factor_inv_2 = power_mod(invmod2, count_factor_5, c)

m = 1
for i in range(1, c+1):
    if i % 5 == 0:
        continue
    m = (m * i) % c

non_multiple_5_factorial_c = m

cache = {}

def calculate_n_dividing_all_factor_5(n, c, start = 1):
    if n <= c:
        m = 1
        for i in range(start, start + n):
            while i % 5 == 0:
                i = i // 5
            m = (m * i) % c
        return m

    if n in cache:
        return cache[n]

    p = n // c
    q = n % c
    p5 = (p * c) // 5 # number of multiples of 5 in p!
    if not p5 in cache:
        m_p5 = calculate_n_dividing_all_factor_5(p5, c)
    if not q in cache:
        m_q = calculate_n_dividing_all_factor_5(q, c, start = (n // c) * c + 1)
    m = power_mod(non_multiple_5_factorial_c, p, c)
    m = (m * m_p5) % c
    m = (m * m_q) % c
    cache[n] = m
    return cache[n]

non_factor_5 = calculate_n_dividing_all_factor_5(N, c)

print(crt(0, non_factor_5 * factor_inv_2, 2**5, 5**5))

