# Question: https://projecteuler.net/problem=59

from collections import Counter

with open('inputs/p059_cipher.txt', 'r') as f:
    line = f.readlines()[0].strip().split(',')
    chars = list(map(int, line))

# Heuristics
#f = lambda x: x^(
g = lambda x: chr(x)
v = 'aeion. '
found = False
for k1 in range(ord('a'), ord('z')+1):
    if found:
        break
    for k2 in range(ord('a'), ord('z')+1):
        if found:
            break
        for k3 in range(ord('a'), ord('z')+1):
            key = [k1, k2, k3]
            f = lambda x, key, i: x^key[i%len(key)]
            c = []
            for i, ch in enumerate(chars):
                c.append(f(ch, key, i))
            c = list(map(g, c))
            lower = ''.join(c).lower()
            freq = Counter(lower)
            #if freq.most_common(1)[0][0] == ' ':
            most_common = freq.most_common(3) 
            if most_common[0][0] in v and most_common[1][0] in v:# and most_common[2][0] in v:
                #break
                #if 'the' in lower and 'a ' in lower:
                #    break
                if '3.14' in lower: # solved by inspection
                    found = True
                    break
print(sum(list(map(ord,c))))


