# Question: https://projecteuler.net/problem=137

# https://en.wikipedia.org/wiki/Fibonacci_number#Symbolic_method
# -> sum(F_i * z^i) = z/(1-z-z^2)
# z/(1-z-z^2) = A; A \in N, z \in Q
# z = A - Az - Az^2
# Az^2 + (A+1)z - A = 0
# z = ((-A-1) (+-) sqrt(4A^2+(A+1)^2))/2A
# z = (sqrt(4A^2+(A+1)^2) - (A+1))/2A
# z \in Q when 4A^2+(A+1)^2 = L^2 for some L \in N

# https://www.wolframalpha.com/input/?i=4A%5E2%2B%28A%2B1%29%5E2+%3D+L%5E2+positive+integer+solutions

# A = 1/10 (11 (9 - 4 sqrt(5))^(2 n) - 5 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 11 (9 + 4 sqrt(5))^(2 n) + 5 sqrt(5) (9 + 4 sqrt(5))^(2 n) - 2) and L = 1/10 (25 (9 - 4 sqrt(5))^(2 n) - 11 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 25 (9 + 4 sqrt(5))^(2 n) + 11 sqrt(5) (9 + 4 sqrt(5))^(2 n)) and n>=0 and n element Z
# A = 1/10 ((9 - 4 sqrt(5))^(2 n) - sqrt(5) (9 - 4 sqrt(5))^(2 n) + (9 + 4 sqrt(5))^(2 n) + sqrt(5) (9 + 4 sqrt(5))^(2 n) - 2) and L = 1/10 (5 (9 - 4 sqrt(5))^(2 n) - sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) + sqrt(5) (9 + 4 sqrt(5))^(2 n)) and n>=1 and n element Z
# A = 1/5 (-2 (9 - 4 sqrt(5))^(2 n) - sqrt(5) (9 - 4 sqrt(5))^(2 n) - 2 (9 + 4 sqrt(5))^(2 n) + sqrt(5) (9 + 4 sqrt(5))^(2 n) - 1) and L = 1/5 (5 (9 - 4 sqrt(5))^(2 n) + 2 sqrt(5) (9 - 4 sqrt(5))^(2 n) + 5 (9 + 4 sqrt(5))^(2 n) - 2 sqrt(5) (9 + 4 sqrt(5))^(2 n)) and n>=1 and n element Z

from decimal import *
getcontext().prec = 64

ans = 0

N = 15
sqrt5 = Decimal(5).sqrt()

As = []

A1 = lambda n: Decimal(0.1)*(11*(9-4*sqrt5)**(2*n)-5*sqrt5*(9-4*sqrt5)**(2*n)+11*(9+4*sqrt5)**(2*n)+5*sqrt5*(9+4*sqrt5)**(2*n)-2)
A2 = lambda n: Decimal(0.1)*((9-4*sqrt5)**(2*n)-sqrt5*(9-4*sqrt5)**(2*n)+(9+4*sqrt5)**(2*n)+sqrt5*(9+4*sqrt5)**(2*n)-2)
A3 = lambda n: Decimal(0.2)*(-2*(9-4*sqrt5)**(2*n)-sqrt5*(9-4*sqrt5)**(2*n)-2*(9+4*sqrt5)**(2*n)+sqrt5*(9+4*sqrt5)**(2*n)-1)

for n in range(N+1):
    As.append(int(A1(n)))
    As.append(int(A2(n)))
    As.append(int(A3(n)))

As = sorted(As)
while As[0] < 1:
    del As[0]

print(As[N-1])

