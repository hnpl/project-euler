from sage.all import *

N = 10**15
M = 100
MOD = factorial(15)
mods = {base: base**exp for base, exp in factor(MOD)}

def fibonacci2(n, mod):
    R = IntegerModRing(mod)
    T = matrix(R, 2, 2, lambda x, y: 0)
    T[0] = [0, 1]
    T[1] = [1, 1]
    S = matrix(R, 2, 1, lambda x, y: 0)
    S[0] = 0
    S[1] = 1
    S = (T**n) * S
    return S[0][0], S[1][0]

if __name__ == "__main__":
    ans = 0

    m = {}
    f = {}
    f1 = {}
    for base, exp in factor(MOD):
        base_exp = base ** exp
        R = IntegerModRing(base_exp)
        m[base_exp] = R(0)
        a, b = fibonacci2(N, base_exp)
        f[base_exp] = a
        f1[base_exp] = b

    for x in range(M+1):
        for base, mod in mods.items():
            r = x**2+x-1
            while r % base == 0:
                r = r // base
            r = inverse_mod(r, mod)
            R = IntegerModRing(mod)
            Rx = R(x)
            r = r * (f1[mod] * (Rx ** (N+1)) + f[mod] * (Rx ** (N+2)) - Rx)
            m[mod] += r

    curr_mod = 1
    residue = 0
    for mod, r in m.items():
        residue = crt(residue, lift(r), curr_mod, mod)
        curr_mod *= mod

    ans = residue

    print(ans)
    
