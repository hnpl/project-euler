# Question: https://projecteuler.net/problem=25

a = 1
b = 1
i = 1
while len(str(b)) < 1000:
    a, b = a + b, a
    i = i + 1
print(i)
