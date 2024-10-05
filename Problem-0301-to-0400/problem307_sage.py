# Problem: https://projecteuler.net/problem=307

"""
    . Note that the defects are not the same,
        This means, a chip with (defect 1 and defect 2) is different from that chip with (defect 2 and defect 3),
    
    . Let N be the number of chips, M be the number of defects.
    . Let x be the number of chips having 2 defects.
      This means there are M - 2x chips having 1 defect,
             and there are N - M + x chips with 0 defects.
    
    . The number of ways of choosing x chips from N chips:
        C(N, x)
    . The number of ways of choosing 2x defects from M defects:
        C(M, 2x)
    . The number of ways of assigning the above 2x defects to the above x chips where each chip has exactly 2 defects:
        (2x)! / (2^x)
        Why?
            Suppose there are 2x numbers and a grid of size 2 rows and x columns.
            There are (2x)! ways to assign 2x numbers to 2x squares.
            Let each column represent a chip, then a chip with (defect 1 and defect 2) is the same as that chip with (defect 2 and defect 1).
            So, for each chip, we are double counting the number of assignments.
            So, for each chip, we divide the number of assignments by 2.
            As a result, the number of ways of assigning 2x defects to x chips where each chip has exactly 2 defects is (2x)! / 2^x.
    . So, now we have (N-x) chips to assign (M-2x) defects.
      Note that, now, each chip has at most 1 defects.
      So, it is the same as choosing (M-2x) chips and putting 1 defect on each.
    . The number of ways of choosing (M-2x) chips from (N-x) chips:
        C(N-x, M-2x)
    . The number of ways of assigning (M-2x) defects to (M-2x) chips:
        (M-2x)!

    . So, the number of ways of having x chips with 2 defects each is:
        C(N, x) * C(M, 2x) * (2x)! / (2^x) * C(N-x, M-2x) / (M-2x)!

    . The number of ways of assigning M defects to N chips:
        N**M (there are N options to assign to each defect, so for M defects, there are N**M possible assignments of M defects to N chips)

    . The probability of having a chip with 3 defects equals to ONE minus the probability of having no chips with more than 2 defects.
      So,
        the_answer = 1.0 - sum(x | 0 <= x <= M/2)[C(N, x) * C(M, 2x) * (2x)! / (2^x) * C(N-x, M-2x) / (M-2x)! / N**M]
"""


from sage.all import binomial, factorial, Integer

# Use sagemath's Integer to avoid overflow

N = Integer(1000000)
M = Integer(20000)

N_power_M = N**M

def p(x):
    a = binomial(N,x) * binomial(M,2*x) * factorial(2*x) / (2**x) * binomial(N-x, M-2*x) * factorial(M-2*x)
    return float(a/N_power_M)

if __name__ == "__main__":
    ans = 1.0 - sum([p(x) for x in range(0, M//2 + 1)])
    print(ans)
