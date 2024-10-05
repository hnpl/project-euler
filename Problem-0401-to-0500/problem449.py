# Question: https://projecteuler.net/problem=449

"""
    .  b^2 * x^2 + b^2 * y^2 + a^2 * z^2 = a^2 * b^2
    or x^2 / a^2 + y^2 / a^2 + z^2 / b^2 = 1
    . a = 3, b = 1 (in a normal notation, a = b = 3, c = 1)
    . Volume of the core = Volume of an ellipsoid = 4*pi*a*b*c = 12*pi
    . To calculate the volume of the new shape, we will use the volume formula of "solid of revolution".
      https://en.wikipedia.org/wiki/Solid_of_revolution
    . First, we need to figure out the 2D curve that would be rotated to find the volume.
        . Consider the cut of the original shape on (x,z-plane) at y = 0.
        . The curve formed the cut is governed by,
            x^2 / a^2 + z^2 / b^2 = 1
        . Let parameterize x and z by,
            . x(t) = 3 * sin(t)
            . z(t) =     cos(t)
            for `t` in [0, 2pi]
        . We'll use the formula provided by https://en.wikipedia.org/wiki/Parallel_curve#Parallel_curve_of_a_parametrically_given_curve to find the new curve.
        . Let X(t) and Z(t) to be the functions describing the shape of the new curve. We have, (use d = -1)
            . X(t) = 3*sin(t) - (-sin(t))/(sqrt((3*cos(t))**2 + (sin(t)**2)))
            . Z(t) = cos(t) + (3*cos(t)/(sqrt((3*cos(t))**2 + (sin(t)**2))))
        . The volume of solid of revolution is,
            V = pi * integral(Z^2 * dX)
              = pi * integral(Z(t)^2 * (dX/dt) * dt)
              = pi * integral(Z(t)^2 * X'(t) * dt)
    . Finally, the added volume is,
        dV = V - 12*pi
"""

from sage.all import *

a = var('a')

X = 3*sin(a) - (-sin(a))/(sqrt((3*cos(a))**2 + (sin(a)**2)))
Y = cos(a) + (3*cos(a)/(sqrt((3*cos(a))**2 + (sin(a)**2))))
f = pi * (X**2) * Y.derivative()
V = numerical_integral(f, pi, 0)[0]
dV = V - 12*pi
ans = n(dV)

print(ans)
