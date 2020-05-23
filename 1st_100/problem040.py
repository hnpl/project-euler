# Question: https://projecteuler.net/problem=40

def get_ith_digit_of(n, i):
    return int(str(n)[i])

N = [10**i for i in range(7)]

i = 0
k = 1
prod = 1

lower = 1 # position of the first digit of [10**(k-1), 10**k)
upper = 9 # position of the last digit of [10**(k-1), 10**k)
for n in N:
    while not (n >= lower and n <= upper):
        k = k + 1
        lower = upper + 1
        upper = lower + k * 9 * 10 ** (k - 1) - 1
    num = 10**(k-1) + (n - lower) // k
    digit_pos = (n - lower) % k
    prod = prod * get_ith_digit_of(num, digit_pos)

print(prod)
