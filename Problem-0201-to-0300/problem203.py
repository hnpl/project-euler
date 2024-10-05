# Question: https://projecteuler.net/problem=203

primes = []

with open("inputs/primes_1e6.txt", "r") as f:
    for p in f:
        primes.append(int(p.strip()))

prime_squares = [p*p for p in primes]

def is_squarefree(x):
    for p in prime_squares:
        if p > x:
            return True
        if x % p == 0:
            return False
    return True

s = {1}
row = [1, 1]

for i in range(3, 52):
    row, old_row = [1], row
    for j in range(1,len(old_row)):
        row.append(old_row[j] + old_row[j-1])
        s.add(row[-1])
    row.append(1)
result = sum([x for x in s if is_squarefree(x)])
print(result)
