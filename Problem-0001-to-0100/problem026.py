# Question: https://projecteuler.net/problem=26

N = 1000
max_length = 0
ans = 0
for n in range(2, N):
    r = 1
    while r*10 < n:
        r = r * 10
    chain = set()
    while True:
        if r == 0:
            break
        r = r * 10
        r = r % n
        if r in chain:
            break
        chain.add(r)
    if r == 0:
        continue
    chain_length = len(chain)
    if max_length < chain_length:
        max_length = chain_length
        ans = n
print(ans)
