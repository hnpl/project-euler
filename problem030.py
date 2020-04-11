# Question: https://projecteuler.net/problem=30

def pow5(x):
    return x**5

def sum_digits_pow5(x):
    digits = list(str(x))
    digits = map(int, digits)
    s = sum(map(pow5, digits))
    return s

k = 1

s = 0

while pow5(9) * k >= 10**(k-1):
    k = k + 1

for i in range(2, 10**(k-1)):
    if sum_digits_pow5(i) == i:
        s = s + i
        print(i)

print(s)

