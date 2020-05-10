# Question: https://projecteuler.net/problem=196

from sage.all import *

def get_start_end(row):
    n = row
    start, end = (n-1)*n//2+1, n*(n+1)//2
    return start, end

def in_range(a, start, end):
    return a >= start and a <= end

# If the length of the row containing 'a' is odd
#    x, n(1),    x, n(2),    x
# n(3),    x, n(4),    x, n(5)
# n(6),    x,    a,    x, n(7)
#    x, n(8),    x, n(9),    x
#    x, n(A),    x, n(B),    x
# 
# n(i) -> i'th neighbor as described above
# x -> the neighbor that is an even integer (so it's not in the prime triplet)

def get_neighbors(a, row):
    # with the input we have, we don't encounter corner cases, 
    # where some of the neighbors are not available
    n = []
    # row - 2
    n.append(a - 2*row + 2) # n(1)
    n.append(a - 2*row + 4) # n(2)
    # row - 1
    n.append(a -   row - 1) # n(3)
    n.append(a -   row + 1) # n(4)
    n.append(a -   row + 3) # n(5)
    # row
    n.append(a         - 2) # n(6)
    n.append(a         + 2) # n(7)
    # row + 1
    n.append(a +   row - 1) # n(8)
    n.append(a +   row + 1) # n(9)
    # row + 2
    n.append(a + 2*row    ) # n(A)
    n.append(a + 2*row + 2) # n(B)
    return n

prime_cache = {}
def is_prime_cached(n):
    if not n in prime_cache:
        prime_cache[n] = is_prime(n)    
    return prime_cache[n]

p = lambda n: is_prime_cached(n)

# valid patterns: 14a, 24a, 4a8, 4a9, 8a9, 68a, 79a, a8A, a9B
def check_neighbors(a, row):
    neighbors = [0] + get_neighbors(a, row)
    #          ^^^^^
    # padding here because we generate patterns from a 1-indexed array
    if p(neighbors[4]):
        return p(neighbors[1]) or p(neighbors[2]) or p(neighbors[8]) or p(neighbors[9])
    elif p(neighbors[8]):
        return p(neighbors[9]) or p(neighbors[6]) or p(neighbors[10])
    else:
        return p(neighbors[9]) and (p(neighbors[7]) or p(neighbors[11]))

ans = 0
rows = [5678027, 7208785]
for row in rows:
    start, end = get_start_end(row)
    primes = prime_range(start, end, algorithm="pari_isprime")
    for prime in primes:
        prime_cache[prime] = True
        if check_neighbors(prime, row):
            ans = ans + prime
print(ans)
