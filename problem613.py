# Question: https://projecteuler.net/problem=613
from sage.symbolic.integration.integral import definite_integral
p(x,y) = (3*pi/2 - arctan((40-y)/x) - arctan((30-x)/y))/(2*pi)
I_y(x) = definite_integral(p(x,y),y,0,-4*x/3+40)
I = definite_integral(I_y(x), x, 0, 30)
print(float(I)/600)
