# Question: https://projecteuler.net/problem=120

# The coefficients of a^(odd) cancel out, so there might be a pattern ...

#   n |    X_n = (a-1)^n + (a+1)^n | mod a^2
#-----|----------------------------|--------
#   1 |                         2a | 2a
#   2 |                   2a^2 + 2 | 2
#   3 |                  2a^3 + 6a | 6a
#   4 |            2a^4 + 6a^2 + 2 | 2
#   5 |         2a^5 + 20a^3 + 10a | 10a
#   6 |   2a^6 + 30a^4 + 30a^2 + 2 | 2
#   7 | 2a^7 + 42a^5 + 70a^3 + 14a | 14a

# So, if n is even, X^n = 2 (mod a^2)
#     if n is odd,  X^n = 2na (mod a^2)

# For a given 'a', what is the maximum x such that 2na = x (mod a^2) where n is an abitrary positive integer?
# We know that 2na is even, so if a if odd, the highest possible value of x is a^2 - 1
#                              if a is even, the highest possible value of x is a^2 - 2
# If a is even, then there exists k such that a = 2k. pick n = k, we have 2na = 2ka = a^2 = 0 (mod a^2)
#                                                          n = k - 1, we have 2na = a^2 - 2a (mod a^2)
#                                                          n = k - 2, we have 2na = a^2 - 4a (mod a^2)
#                                                          ...
#                                                          n = k - k, we have 2na = a^2 - 2ka = a^2 - a^2 = 0 (mod a^2)
#                                                     so the modulo group is {0, a^2 - 2ka}
# If a is odd, then there exists k such that a = 2k + 1. Pick n = 2k+1, then 2na = 2(2k+1)a = 2a^2 = 0 (mod a^2)
#                                                             ...
#                                                             n = k+2, then 2na = 2(k+2)a = (2k+1)a + 3a = a^2 + 3a = 3a = a^2 - a^2 + 3a = a^2 - (2k-2)a (mod a^2)
#                                                             n = k+1, then 2na = 2(k+1)a = (2k+1)a + a = a^2 + a = a = a^2 - (2k)a (mod a^2)
#                                              start here ->  n = k, then 2na = 2ka = (2k+1)a - a = a^2 - a (mod a^2)
#                                                             n = k-1, then 2na = 2(k-1)a = (2k+1)a - 3a = a^2 - 3a (mod a^2)
#                                                             n = k-2, then 2na = 2(k-2)a = (2k+1)a - 5a = a^2 - 5a (mod a^2)
#                                                             ...
#                                                             n = k-k, then 2na = 0 (mod a^2)
#                                                        so the modulo group is {0, a^2 - ka}

# So, if 'a' is odd, r_max = max(2, a^2 - a). Since a >= 3, r_max = a^2 - a
#     if 'a' is even, r_max = max(2, a^2 - 2a). Since a >= 3, r_max = a^2 - 2a
# So, sum_{3,n}(r_max) = [sum_{1,n}(a^2-a)] - [sum_{3<=a<=n, 'a' even} (a)] - {a=1}(a^2-a) - {a=2}(a^2-a) 
#                      = [sum_{1,n}(a^2-a)] - (2*[sum_{1<=i<=floor(n/2)} (i)] - 2) - {a=1}(a^2-a) - {a=2}(a^2-a)
#                      = 1/6 * n * (n+1) * (2n+1) - 1/2 * n * (n+1) - (2*n/2*(n/2+1) - 2) - 0 - 2 
#                      = 1/3 * (n-1) * n * (n+1) - 1/4*n*(n+2)

N = 1000
result = (N-1)*N*(N+1) // 3 - N * (N+2)//4
print(result)
