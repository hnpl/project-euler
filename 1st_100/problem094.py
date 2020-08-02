# Problem: https://projecteuler.net/problem=94

"""
    Suppose we have a triangle that has he two side lengths, supposedly 'a', and the third side of length 'b'.
    The area of the triangle is S = 1/2 * h * b.
    We have that S and b are integral, so h must be integral.
    We also have that a^2 = h^2 + b^2 / 4.

    Case 1: b = a + 1
        Then we have a^2 = h^2 + (a+1)^2 / 4.
        Using WolframAlpha, we can find the generating functions of the above function integral solutions.
        -> https://www.wolframalpha.com/input/?i=a%5E2+%3D+h%5E2+%2B+%28a%2B1%29%5E2+%2F+4+integral+solutions
        Out of the 4 pair of functions for 'a' and 'h', only one of them generates positive 'a' and 'h'.
        Since we calculting the perimeter, we only need to generate 'a'.
        The function for 'a' is, https://www.wolframalpha.com/input/?i=1%2F3+%281+%2B+%287+-+4+sqrt%283%29%29%5En+%2B+%287+%2B+4+sqrt%283%29%29%5En%29&assumption=%22ClashPrefs%22+-%3E+%7B%22Math%22%7D

    Case 2: b = a - 1
        Similar to case 1.
        -> https://www.wolframalpha.com/input/?i=a%5E2+%3D+h%5E2+%2B+%28a-1%29%5E2+%2F+4+integral+solutions
        -> https://www.wolframalpha.com/input/?i=1%2F3+%28-1+%2B+%287+-+4+sqrt%283%29%29%5En+%282+%2B+sqrt%283%29%29+-+%28-2+%2B+sqrt%283%29%29+%287+%2B+4+sqrt%283%29%29%5En%29&assumption=%22ClashPrefs%22+-%3E+%7B%22Math%22%7D

    We use SageMath's symbolic expression class to get the exact results.
"""

from sage.all import *

N = 10**9

n = var('n')
c = var('c')

ans = 0

# b = a + 1
#def height(n, c):
#    return (((7+4*sqrt(c))**n - (7-4*sqrt(c))**n) / (2*sqrt(c))).expand()
def side(n, c):
    return (((7-4*sqrt(c))**n + (7+4*sqrt(c))**n + 1) / 3).expand()

i = 0
while True:
    i = i + 1
    #h = height(n = i, c = 3)
    a = side(n = i, c = 3)
    P = int(a+a+a+1)
    if P > N:
        break
    ans = ans + P

# b = a - 1
#def height(n, c):
#    return (((2*sqrt(c)-3)*((7+4*sqrt(3))**n) - (2*sqrt(c)+3)*((7-4*sqrt(3))**n))/6).expand()
def side(n, c):
    return (((2+sqrt(c))*((7-4*sqrt(c))**n) - (sqrt(c)-2)*((7+4*sqrt(c))**n) - 1)/3).expand()
i = 0
while True:
    i = i + 1
    #h = height(n = i, c = 3)
    a = side(n = i, c = 3)
    if (a == 1):    # a = 1 -> b = 0
        continue
    P = int(a+a+a-1)
    if P > N:
        break
    ans = ans + P
print(ans)
