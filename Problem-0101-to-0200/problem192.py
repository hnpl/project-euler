# Problem: https://projecteuler.net/problem=192

from sage.all import *

N = 100000
M = 10**12
PREC = 180 # increase this until getting the correct answer

if __name__ == "__main__":
    ans = 0

    for n in range(2, N+1):
        if NN(n).is_square():
            continue
        ans += sqrt(n).n(PREC).nearby_rational(max_denominator = M).denominator()

    print(ans)