# https://projecteuler.net/problem=577

N = 12345

import numpy as np

# number of points of unfilled equilateral triangles of size i constructed from dots
# T[1] = 1, T[2] = 3, T[3] = 6, T[4] = 9, T[5] = 12
T = np.arange(N+2, dtype = np.int64)
T -= 1
T *= 3
T[0] = 0
T[1] = 1
# filled triangles
# Note that
# T[9] contains T[6], which in turns contains T[3]
# Similarly, T[8] contains T[5], which in turns contains T[2]
# T[7] contains T[4], which in turns contains T[1]
T[1::3] = np.cumsum(T[1::3])
T[2::3] = np.cumsum(T[2::3])
T[3::3] = np.cumsum(T[3::3])


def H(n):
    ans = 0
    n = n + 1 - 3
    side = 2
    while n > 0:
        # for every hexagon of size k, we can rotate and get k-2 others hexagon
        ans = ans + T[n] * (side-1)
        n = n - 3
        side = side + 1
    return ans

def H_fast(n):
    n = n + 1 - 3
    ans = np.multiply(T[n:0:-3], np.arange(0, (n+2)//3, 1) + 1) 
    return sum(ans)

ans = 0
for n in range(3, N+1):
    ans = ans + H_fast(n)
    #assert(H(n) == H_fast)
print(ans)