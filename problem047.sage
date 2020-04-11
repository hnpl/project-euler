# Question: https://projecteuler.net/problem=47
i = 2 * 3 * 5 * 7 - 1
count = 0

while count < 4:
    i = i + 1
    n_prime_factors = len(factor(i))
    if n_prime_factors == 4:
        count = count + 1
    else:
        count = 0
print(i-3)
