# Question: https://projecteuler.net/problem=479

# - We have that,
#    1/x = (k/x)^2(k+x^2)-kx
# ->   x = k^2(k+x^2)-kx^3
# ->   x = k^3 + k^2x^2 - kx^3
# -> kx^3 - k^2x^2 + x - k^3 = 0
# ->  x^3 - kx^2 + (1/k)x - k^2 = 0 [1]
#
# - Since a, b, and c are root of [1], we also have that, if x satisfies [1], then
#    (x - a)(x - b)(x - c) = 0 (note that the coefficients of this equation are similar to those of expanding (a+b)(b+c)(c+a)
# -> x^3 - (a+b+c)x^2 + (a*b + b*c + c*a)x - a*b*c = 0 [2]
#
# - From [1] and [2], we have that,
#   . a + b + c = k                 [3]
#   . a*b + b*c + c*a = 1/k         [4]
#   . a * b * c = k^2               [5]
#
# - Now, consider expanding (a + b)(b + c)(c + a)
#                         = a^2*b + a*b^2 + b^2*c + b*c^2 +c^2*a + c*a^2 + 2*a*b*c [6]
#   Now we need to evaluate the term (a^2*b + a*b^2 + b^2*c + b*c^2 +c^2*a + c*a^2).
#   Note that, if we multiply [3] and [4], we have the terms that are similar to what we want to evaluate.
#   Multiply [3] and [4], we have,
#       (a + b + c)(a*b + b*c + c*a) = 1
#    -> a^2*b + a*b^2 + b^2*c + b*c^2 +c^2*a + c*a^2 + 3*a*b*c = 1
#    -> a^2*b + a*b^2 + b^2*c + b*c^2 +c^2*a + c*a^2 = 1 - 3*a*b*c [7]
#   Replace [7] to [6], we have,
#       (a + b)(b + c)(c + a) = (1 - 3*a*b*c) + 2*a*b*c
#                             = 1 - a*b*c
#                             = 1 - k^2 [8]
# - So, S(n) = sum_{p=1..n} sum_{k=1..n} (1-k^2)^p
#            = sum_{k=1..n} sum_{p=1..n} (1-k^2)^p (exchanging summation order)
#   Note that sum_{p=1..n} (1-k^2)^p is a partial sum of a power series, so,
#       S(n) = sum_{k=1..n} [ (1-(1-k^2)^(n+1)) / (1-1+k^2) ]
#            = sum_{k=1..n} [ (1-(1-k^2)^(n+1)) / k^2 - 1]
# - From here, we can iterate over k to find the sum.

from sage.all import *

N = 10**6
MOD = 10**9 + 7

if __name__ == "__main__":
    Z = IntegerModRing(MOD)
    S = Z(0)
    for k in range(1,N+1):
        numerator = Z(1) - (Z(1) - Z(k)**2)**(N+1)
        denominator = Z(k)**2
        S += numerator / denominator - 1
    print(S)
