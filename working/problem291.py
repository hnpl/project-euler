from sage.all import is_pseudoprime

N = 5 * (10 ** 15)

f = lambda n: n**2 + (n+1)**2
ans = 0
n = 1
while True:
    p = f(n)
    if p > N:
        break
    if is_pseudoprime(p):
        ans += 1
        print(p, ans)
    n += 1
print(ans)
