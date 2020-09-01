# Problem: https://projecteuler.net/problem=216

from sage.all import is_pseudoprime

N = 50000000

if __name__ == "__main__":
    ans = 0

    for n in range(2, N+1):
        if is_pseudoprime(2*n*n-1):
            ans = ans + 1

    print(ans)
