# Question: https://projecteuler.net/problem=41

import itertools
found = False
for n in range(9, 0, -1):
    if sum(list(range(n+1))) % 3 == 0:
        continue
    print(n)
    for i in itertools.permutations(list(range(n, 0, -1))):
        n = int(''.join(map(str,i)))
        if is_prime(n):
            found = True
            break
    if found:
        break
print(n)
