# Question: https://projecteuler.net/problem=86

# The problem says there are 3 shortest path candidates; we only need to determine whether "the shortest path" is an integer.
# Suppose a, b, c are the dimension of the cuboid, and a <= b <= c.
# The shortest path must be c^2 + (a+b)^2.
# Proof:
#   Let a <= b <= c.
#   The square of shortest path candidates: x = a^2 + (b+c)^2, y = b^2 + (a+c)^2, z = c^2 + (a+b)^2.
#   z - y = 2ab - 2ac = 2a(b-c) <= 0; so z <= y
#   z - x = 2ab - 2bc = 2b(a-c) <= 0; so z <= x
#   So, if a <= b <= c, then the shortest path must has the length of c^2 + (a+b)^2.
# ---
# So, if the cuboid of sides of shorter than M has DP[M-1] cuboids that has an integer as the shortest path, how do we calculate DP[M]?
# Let a <= b <= c = M. Suppose we know that sqrt(c^2 + y^2) is an integer, c = M, y <= 2M. Suppose a + b = y <= 2M.
#   If a + b = y <= M, 
#     then (a,b) in {(1, y-1), (2, y-2), ..., (floor(y/2),ceil(y/2))} => floor(y/2) pairs of (a,b) for each y.
#   If a + b = y > M,
#     then (a,b) in {(y-M, M), (y-M+1, M-1), (y-M+2, M-2), ..., (floor(y/2), ceil(y/2)} => floor(y/2) - y + M + 1 pairs of (a,b) for each y.

N = 10000 
G = 10**6

squares = [i*i for i in range(2*N+1)]
square_set = set(squares)

M = 1

DP = [0]
while DP[-1] < G:
    M = M + 1
    squareM = squares[M]
    count = 0
    for y in range(2, M+1):
        if squareM + squares[y] in square_set:
            count = count + y//2
    for y in range(M+1, 2*M+1):
        if squareM + squares[y] in square_set:
            count = count + y//2 - y + M + 1
    DP.append(DP[-1] + count)

assert(DP[99-1] == 1975)
assert(DP[100-1] == 2060)
print(len(DP))
