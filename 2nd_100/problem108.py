# Question: https://projecteuler.net/problem=108

"""
1/x + 1/y = 1/n
Since n > 0, we have x > n and y > n. Assume x >= y.
Let x = n + a, y = n + b.
   1/x + 1/y = 1/n
-> xn + yn = xy
-> (n+a)n + (n+b)n = (n+a)(n+b)
-> 2n^2 + an + bn = n^2 + an + bn + ab
-> n^2 = ab
The number of solutions of (x,y) for each n is the number of solutions of (a,b) for each n.
The number of positive integers (a,b) where a >= b that ab = n^2 is (m-1)/2+1, where m is the number of divisors of n^2.
Why?
    | Let the number of divisors be 'm'.
    | For each divisor d where d != n, let (a, b) = (d, n^2/d).
    |   Note that, (d, n^2/d) is the same as (n^2/d, d), where n^2/d is also a divisor but only one of them have a > b.
    |   There are (m-1) such a case, therefore there are (m-1)/2 pair of (a,b) that a > b.
    | For the case (a, b) = (n, n); there is only one such a case.
    | So, in total, there are (m-1)/2+1 pairs of (a,b).


In this problem, we want to find the smallest n such that (number_of_divisors of n**2) >= 1999.
"""

from sage.all import number_of_divisors

ans = 0
while True:
    ans = ans + 1
    if number_of_divisors(ans**2) >= 1999:
        break
print(ans)
