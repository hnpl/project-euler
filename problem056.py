# Question: https://projecteuler.net/problem=56

m = 0

def sum_digits(n):
    return sum(list(map(int, str(n))))

for i in range(1, 101):
    for j in range(1, 101):
        m = max(m, sum_digits(i**j)) # fast enough

print(m)
