# Problem: https://projecteuler.net/problem=226

"""
    - We need to find the coordinates of the intersections between the circle and the curve.
      We have one intersection at (0.5, 0.5)
      The other intersection is on the third quarter of the circle.

    - The calculations related to the Blancmange are described here
        https://en.wikipedia.org/wiki/Blancmange_curve

    - The equation of the lower half of the circle is:
        (the circle of radius 1/4 centered at (0,0))
            X^2 + Y^2 = 1/16
        (the lower half of the circle of radius 1/4 centered at (0,0))
            Y = -sqrt(1/16 - X^2)
        (the lower half of the circle of radius 1/4 centered at (1/4, 1/2))
            Let X = x - 1/4, Y = y - 1/2, then,
            y - 1/2 = -sqrt(1/16 - (x - 1/4)^2)
         or y = 1/2 - sqrt(1/16 - (x - 1/4)^2)

    - We search x in range of [0, 0.5) to find the intersection on the left.

    - Use integral to find the overlapping area.
"""

from sage.all import *
from sage.symbolic.integration.integral import definite_integral

def circle(x):
    X = x - 1/4
    Y = -sqrt(1/16-X**2)
    y = Y + 1/2
    return float(y)

def s(n):
    return abs(n - round(n))

def blanc(x, tol = 1e-10):
    w = float(s((2**0) * x) / (2**0))
    prev_w = 0
    delta = -1
    n = 0
    while abs(delta) > tol:
        n += 1
        prev_w = w
        delta = s((2**n)*x)/(2**n)
        w = prev_w + delta
    return float(w)

# https://en.wikipedia.org/wiki/Blancmange_curve#Integrating_the_Blancmange_curve
def I(x, depth, max_depth = 990):
    if depth > max_depth:
        return 0
    if x <= 1/2:
        return I(2*x, depth + 1)/4 + x**2 / 2
    elif x <= 1:
        return 1/2 - I(1-x, depth + 1)
    else:
        n = int(x)
        return n / 2 + I(x - n, depth + 1)

def find_x(x_i, x_f, step, tol = 1e-15):
    x = x_i
    y1 = 1
    y2 = 0
    while True:
        x = x + step
        y1 = circle(x)
        y2 = blanc(x)
        if y2 > y1: # now we on the right of the intersection, we take a step back and use a smaller step
            x = x - step
            step = step / 10
            continue
        if abs(y1-y2) < tol:
            break
    return x

if __name__ == "__main__":
    x = find_x(0, 1/2, 0.1)
    I_blanc = I(0.5, 0) - I(x, 0)
    k = var('k')
    I_circle = definite_integral(1/2-sqrt(1/16-(k-1/4)**2),k,x,0.5)
    print(I_blanc - I_circle)