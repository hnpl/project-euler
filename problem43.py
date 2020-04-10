# Question: https://projecteuler.net/problem=43

import itertools

primes = [2,3,5,7,11,13,17]

sum = 0

# 5 | d4d5d6 -> d6 is either 0 or 5

# d6 = 0
for i in itertools.permutations(list('123456789')):
    num = ''.join(i[:5]) + '0' + ''.join(i[5:])
    valid = True
    for j, prime in enumerate(primes):
        if not int(num[1+j:4+j]) % prime == 0:
            valid = False
            break
    if valid:
        sum = sum + int(num)

# d6 = 5
for i in itertools.permutations(list('012346789')):
    num = ''.join(i[:5]) + '5' + ''.join(i[5:])
    valid = True
    for j, prime in enumerate(primes):
        if not int(num[1+j:4+j]) % prime == 0:
            valid = False
            break
    if valid:
        sum = sum + int(num)

print(sum)

