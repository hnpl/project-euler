# Question: https://projecteuler.net/problem=58

i = 2
n_primes = 3
n = 5
while n_primes / n >= 0.1:
    n = n + 4
    i = i + 1
    i2 = i**2
    if is_prime(4*i2 -8*i + 5):
        n_primes = n_primes + 1
    if is_prime(4*i2 - 10*i + 7):
        n_primes = n_primes + 1
    if is_prime(4*i2 - 6*i + 3):
        n_primes = n_primes + 1

print(i*2-1)
