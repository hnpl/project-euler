# Question: https://projecteuler.net/problem=138

# https://www.wolframalpha.com/input/?i=1+-+2+b+%2B+%285+b%5E2%29%2F4+%3D+L%5E2++integer+solutions
# b = 1/5 (2 (9 - 4 sqrt(5))^(2 n) + sqrt(5) (9 - 4 sqrt(5))^(2 n) + 2 (9 + 4 sqrt(5))^(2 n) - sqrt(5) (9 + 4 sqrt(5))^(2 n) - 4), L = ± 1/10 (5 (9 - 4 sqrt(5))^(2 n) + 2 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) - 2 sqrt(5) (9 + 4 sqrt(5))^(2 n)), n element Z, n>=0

# b = 1/5 (2 (9 - 4 sqrt(5))^(2 n) - sqrt(5) (9 - 4 sqrt(5))^(2 n) + 2 (9 + 4 sqrt(5))^(2 n) + sqrt(5) (9 + 4 sqrt(5))^(2 n) - 4), L = ± 1/10 (5 (9 - 4 sqrt(5))^(2 n) - 2 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) + 2 sqrt(5) (9 + 4 sqrt(5))^(2 n)), n element Z, n>=0

# https://www.wolframalpha.com/input/?i=1+%2B+2+b+%2B+%285+b%5E2%29%2F4+%3D+L%5E2++integer+solutions
# b = 1/5 (-2 (9 - 4 sqrt(5))^(2 n) + sqrt(5) (9 - 4 sqrt(5))^(2 n) - 2 (9 + 4 sqrt(5))^(2 n) - sqrt(5) (9 + 4 sqrt(5))^(2 n) + 4), L = ± 1/10 (5 (9 - 4 sqrt(5))^(2 n) - 2 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) + 2 sqrt(5) (9 + 4 sqrt(5))^(2 n)), n element Z, n>=0

# b = 1/5 (-2 (9 - 4 sqrt(5))^(2 n) - sqrt(5) (9 - 4 sqrt(5))^(2 n) - 2 (9 + 4 sqrt(5))^(2 n) + sqrt(5) (9 + 4 sqrt(5))^(2 n) + 4), L = ± 1/10 (5 (9 - 4 sqrt(5))^(2 n) + 2 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) - 2 sqrt(5) (9 + 4 sqrt(5))^(2 n)), n element Z, n>=0

from decimal import *
getcontext().prec = 64

sqrt5 = Decimal(5).sqrt()

ans = 0

N = 12

bs = []
Ls = []

b1 = lambda n: Decimal(0.2) * (2*(9-4*sqrt5)**(2*n)+sqrt5*(9-4*sqrt5)**(2*n)+2*(9+4*sqrt5)**(2*n)-sqrt5*(9+4*sqrt5)**(2*n) - 4)
b2 = lambda n: Decimal(0.2) * (2*(9-4*sqrt5)**(2*n)-sqrt5*(9-4*sqrt5)**(2*n)+2*(9+4*sqrt5)**(2*n)+sqrt5*(9+4*sqrt5)**(2*n) - 4)
b3 = lambda n: Decimal(0.2) * (-2*(9-4*sqrt5)**(2*n)+sqrt5*(9-4*sqrt5)**(2*n)-2*(9+4*sqrt5)**(2*n)-sqrt5*(9+4*sqrt5)**(2*n) + 4)
b4 = lambda n: Decimal(0.2) * (-2*(9-4*sqrt5)**(2*n)-sqrt5*(9-4*sqrt5)**(2*n)-2*(9+4*sqrt5)**(2*n)+sqrt5*(9+4*sqrt5)**(2*n) + 4)
L1 = lambda n: Decimal(0.1) * (5*(9-4*sqrt5)**(2*n) + 2*sqrt5*(9-4*sqrt5)**(2*n)+5*(9+4*sqrt5)**(2*n)-2*sqrt5*(9+4*sqrt5)**(2*n))
L2 = lambda n: Decimal(0.1) * (5*(9-4*sqrt5)**(2*n) - 2*sqrt5*(9-4*sqrt5)**(2*n)+5*(9+4*sqrt5)**(2*n)+2*sqrt5*(9+4*sqrt5)**(2*n))
L3 = lambda n: L2(n)
L4 = lambda n: L1(n)

for i in range(1, N+1):
    if b1(n=i) > 0:
        Ls.append(int(L1(n=i)))
    if b2(n=i) > 0:
        Ls.append(int(L2(n=i)))
    if b3(n=i) > 0:
        Ls.append(int(L3(n=i)))
    if b4(n=i) > 0:
        Ls.append(int(L4(n=i)))

Ls = sorted(Ls)
print(sum(Ls[:N]))
