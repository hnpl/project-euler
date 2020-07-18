# Question: https://projecteuler.net/problem=190

"""
We have x1, x2, ..., xm > 0.
So, utilizing the AM-GM inquality,
           x1 * (x2)^2 * (x3)^3 * ... * (xm)^m 
         = x1 * [(1/2)(x2)]^2 * [(1/3)(x3)]^3 * ... * [(1/m)(xm)^m] * product{i=1..m}(i^i)
(AM-GM) <= (m / (m(m+1)/2))^(m(m+1)/2) * product{i=1..m}(i^i)
         = (2/(m+1))^(m(m+1)/2) * product{i=1..m}(i^i)
'=' occurs when x1 = (1/2)*x2 = (1/3)*x3 = ... = (1/m)*xm
In other words, x2 = 2*x1, x3 = 3*x1, ..., xm = m*x1.
"""

N = 15

ans = 0

product_i_i = 1

for m in range(2, N+1):
    product_i_i = product_i_i * (m**m)
    Pm = product_i_i * (2/(m+1))**(m*(m+1)//2)
    ans = ans + int(Pm)

print(ans)


