# Question: https://projecteuler.net/problem=90

import itertools

# 6 and 9 are functionally the same, so replace 9 by 6
targets = ['01', '04', '06', '16', '25', '36', '46', '64', '81']

count = 0

for d1 in itertools.combinations(list('0123456678'), 6):
    for d2 in itertools.combinations(list('0123456678'), 6):
        if int(''.join(d1)) <= int(''.join(d2)):
            break
        valid = True
        for n in targets:
            if (not n[0] in d1 or not n[1] in d2) and (not n[0] in d2 or not n[1] in d1):
                valid = False
                break
        if valid:
            count = count + 1

print(count)
