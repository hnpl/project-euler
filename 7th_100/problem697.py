# Question: https://projecteuler.net/problem=697

"""
    . Let n be the number of random variables U_i.
    . We want to find c such that,
        Pr(c * U_1 * U_2 * ... * U_n < 1) = 0.25
      or,
        Pr(c * U_1 * U_2 * ... * U_n > 1) = 0.75
      or,
        Pr[ln(c) + ln(U_1) + ln(U_2) + ... + ln(U_n) > 0] = 0.75
      or,
        Pr[ln(U_1) + ln(U_2) + ... + ln(U_n) > -ln(c)] = 0.75
      or,
        Pr[-ln(U_1) - ln(U_2) - ... - ln(U_n) < ln(c)] = 0.75
    . From this answer here https://math.stackexchange.com/a/3079134, we have that,
      Pr(sum(-ln(U) < c)) follows the Gamma distribution with k = n, theta = 1,
                             i.e. the Gamma distribution with alpha = n, beta = 1
    . cdf = cumulative_distribution_function
    . So, Pr(sum(-ln(U)) < c) = gamma.cdf(x = c, alpha = n)
    . We know that, for a fixed variable n,
      * gamma.cdf(x=c, alpha=n) increases monotonically as variable c increases
    . This means that we can use binary search to find c.

"""

from scipy.stats import gamma
from numpy import log, log10, e

N = 10**7
P = 0.25

if __name__ == "__main__":
    target = 1.0 - P
    a = N

    l = 0
    r = 2*N

    m = 0

    while abs(l - r) >= 0.000001:
        m = (l + r) / 2
        cdf = gamma.cdf(m, a)
        print(m, cdf)
        if cdf > target:
            r = m
        else:
            l = m

    #ans = m / log(10)
    ans = m * log10(e) # m = ln(c), so m * log10(e) = log10(c)
    print(ans)

