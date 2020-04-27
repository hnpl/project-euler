# Question: https://projecteuler.net/problem=100

# Number of blue discs: x
# Total number of discs: a

# x/a * (x-1)/(a-1) = 1/2
# 2x^2 - 2x - a^2 + a = 0 <- this screams "Diophantine equation"

# https://math.stackexchange.com/questions/1414779/solving-a-quadratic-diophantine-equation

from sage.all import *

var('x,a')
assume(x, 'integer')
assume(a, 'integer')
eq = (2*(x**2) - 2*x - a**2 + a == 0)
f = Expression.solve_diophantine(eq, solution_dict = True)
l = 0
f_a = f[l][a]
if f_a(1).n() < 0:
    l = 1
    f_a = f[l][a]
n = 0
N = 10**12
while True:
    v = f_a(n).n()
    print(n, v)
    if v >= N:
        break
    n = n + 1
a = int(f_a(n).n())

f_x = f[l][x]

result = f_x(n).n()

print(int(result)) # This produces off by 1 result as the result is originally of a floating point type

# Wolfram Alpha has the exact answer
# https://www.wolframalpha.com/input/?i=diophantine+equation+%282x%5E2-2x-y%5E2%2By%29
