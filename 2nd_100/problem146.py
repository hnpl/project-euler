# Problem: https://projecteuler.net/problem=146

"""
    . Use sagemath's is_pseudoprime() for fast primality checking
        . It's safe to use is_pseudoprime() for integers <= 10**17 because it uses Baillie-PSW probabilistic primality test, and there are no Baillie-PSW pseudoprimes up to 10**17 passing the composite test (https://mathworld.wolfram.com/Baillie-PSWPrimalityTest.html).

    . Reducing the search space:
        . All primes (except 2) are odd, so n must be even.
        . The last digit of n must be 0 because,
            . the last digit of n   |   n^2 (mod 5) | comments
                                2   |             4 | (n^2+1) (mod 5) = 0
                                4   |             1 | (n^2+9) (mod 5) = 0
                                6   |             1 | (n^2+9) (mod 5) = 0
                                8   |             4 | (n^2+1) (mod 5) = 0
        . n must not be divisible by 3 since if n is divisible by 3, so is n^2+3.
"""

from sage.all import is_pseudoprime

N = 150000000

def pseudo_prime_check(n):
    n_square = n**2
    return is_pseudoprime(n_square + 1) and \
           is_pseudoprime(n_square + 3) and \
           is_pseudoprime(n_square + 7) and \
           is_pseudoprime(n_square + 9) and \
           is_pseudoprime(n_square + 13) and \
           not is_pseudoprime(n_square + 15) and \
           not is_pseudoprime(n_square + 19) and \
           not is_pseudoprime(n_square + 21) and \
           not is_pseudoprime(n_square + 25) and \
           is_pseudoprime(n_square + 27)

if __name__ == "__main__":
    ans = 0
    for n in range(10, N, 10):
        if n % 3 == 0:
            continue
        if pseudo_prime_check(n):
            ans += n
    print(ans)
