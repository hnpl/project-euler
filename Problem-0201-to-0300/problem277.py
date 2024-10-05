# Question: https://projecteuler.net/problem=277

from sage.all import *

x = var('x')

steps = "UDDDUdddDDUDDddDdDddDDUDDdUUDd"

f = [x]
for step in steps[::-1]:
    if step == 'D':
        f.append(f[-1] * 3)
    elif step == 'U':
        f.append((f[-1]*3-2)/4)
    else:
        f.append((f[-1]*3+1)/2)


# we have (ax + b)/c > 10**15, where a and b are constants
numerator = f[-1].numerator()
a = int(numerator.coefficient(x, 1))
b = int(numerator.coefficient(x, 0))
c = int(f[-1].denominator())

# find the lower bound of x such that (ax + b)/c > 10**15
sol = solve([f[-1] == 10**15], x, solution_dict = True)
x = ceil(sol[0][x])

# we need to find x such that ax = -b (mod c)
# we have that gcd(a, c) = 1
z = inverse_mod(a, c) # now we have az = 1 (mod c)
z = z * (-b)          # now we have az = -b (mod c)
z = z % c
while z < x:
    z = z + c
ans = f[-1](x = z)
print(ans)