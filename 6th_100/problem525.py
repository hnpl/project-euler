# Question: https://projecteuler.net/problem=525

"""
    . Ref: https://mathcurve.com/courbes2d.gb/sturm/sturm.shtml
    . We have the formula:
        ds/dt = b * sqrt(1-e^2*(cos(t))^2) / (1 - e^2 * (sin(t))^2)
        where e is the eccentricity of the ellipse
              b < a
    . Note that, the arclength is
        L = integral(ds) = integral((ds/dt), dt)
"""

from sage.all import *

def C(a, b):
    c = sqrt(a**2 - b**2) 
    e = c / a
    x = var('x')
    return numerical_integral(b*sqrt(1-(e**2)*(cos(x)**2))/(1-(e**2)*(sin(x)**2)), 0, pi, eps_abs=1e-10)[0] * 2

print(C(4,1) + C(4,3))
