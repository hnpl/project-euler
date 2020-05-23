# Question: https://projecteuler.net/problem=21

# find a, b such that d(a) = b and d(b) = a

N = 9999

primes = []
prime_set = set(primes)

with open("inputs/primes_1e6.txt", "r") as f:
    for line in f:
        if int(line.strip()) > N:
            break
        primes.append(int(line.strip()))

# Note that, here we store sum_of_divisors[p] includes p itself, while the problem asks for the sum of divisors without p

sum_of_divisors = [0] * (N + 1)
sum_of_divisors[1] = 1

for p in primes:
    sum_of_divisors[p] = p + 1
    j = 2
    while p**j <= N:
        sum_of_divisors[p**j] = sum_of_divisors[p**(j-1)] + p**j
        j = j + 1

# if a and b are coprimes then sum_of_divisors[a*b] = sum_of_divisors[a] * sum_of_divisors[b]
for i in range(2, N+1):
    if not i in prime_set:
        for j in primes:
            if i % j == 0:
                a = i // j
                b = j
                while a % j == 0:
                    a = a // j
                    b = b * j
                sum_of_divisors[i] = sum_of_divisors[a] * sum_of_divisors[b] 

result = 0

for i in range(2, N+1):
    j = sum_of_divisors[i] - i
    if j == i:
        continue
    if j <= N and i == sum_of_divisors[j] - j:
        result = result + i
        
assert(sum_of_divisors[220] == 284 + 220)
assert(sum_of_divisors[284] == 220 + 284)

print(result)