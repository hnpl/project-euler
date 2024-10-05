from sage.all import *

def add_factors(factors1, factors2):
    x = {}
    for base, exp in factors1.items():
        x[base] = exp
    for base, exp in factors2:
        if not base in x:
            x[base] = 0
        x[base] += exp
    return x

def solve(n):
    f = {}
    s = {}
    for i in range(2, n+1):
        f = add_factors(f, factor(i))
        s[i] = dict(f)
    ans = 0
    for k, v in s.items():
        p = 1
        for base, exp in v.items():
            p *= (2*exp+1)
        print(k, p % (10**9+87))
        ans += p
    print(s[56])
    return ans



ans = solve(100)
print(ans)
print(ans % (10**9+87))
