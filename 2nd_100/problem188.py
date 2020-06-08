# Question: https://projecteuler.net/problem=188

"""
A MOD 2^8 = x
A MOD 5^8 = y

A = 2^8p + x
A = 5^8q + y

5^8 A = 10^8 p + 5^8 x
2^8 A = 10^8 q + 2^8 y

(5^8-2^8)A = (5^8x - 2^8y) mod 10^8
30909729(5^8-2^8)A = 30909729(5^8x - 2^8y) mod 10^8
A = 30909729(5^8x - 2^8y) mod 10^8
    ^^^^^^^^
    inverse_mod(5^8-2^8, 10^8) <- SageMath


-- MOD 2^8
Mod(1777, 2^8).multiplicative_order() = 16
-> [1777^(16k)] mod (2^8) = 1
Since 1777 mod 16 = 1
-> (1777^x) mod 16 = 1 forall x
-> 1777↑↑1854 mod 16 = 1
-> 1777↑↑1855 mod 2^8 = 1777^(1777↑↑1854) mod 2^8 = 1777 mod 2^8 = 241

-- MOD 5^8
Mod(1777, 5**8).multiplicative_order() = 312500
-> 1777^(312500k) mod 5^8 = 1


-- MOD 10^8
Mod(1777, 10**8).multiplicative_order() = 1250000
-> 1777^(1250000k) mod 10^8 = 1
"""


B = 2**8
C = 5**8

A_mod_B = 241

cached_A_mod_C = {}
def find_A_mod_C(A_mod_C):
    if not A_mod_C in cached_A_mod_C:
        cached_A_mod_C[A_mod_C] = (1777**(A_mod_C%62500)) % (312500)
    return cached_A_mod_C[A_mod_C]

A_mod_C = 1777
for i in range(2, 1854+1):
    A_mod_C = find_A_mod_C(A_mod_C)
A_mod_C = (1777**A_mod_C) % 5**8

ans = (30909729*((5**8) * A_mod_B - (2**8) * A_mod_C)) % 10**8

print(ans)


""" slow
C = 1250000 # this is too big

cached_A_mod_C = {}
def find_A_mod_C(A_mod_C):
    if not A_mod_C in cached_A_mod_C:
        cached_A_mod_C[A_mod_C] = (1777**A_mod_C) % C
    return cached_A_mod_C[A_mod_C]
A_mod_C = 1777
for i in range(2, 1854+1):
    A_mod_C = find_A_mod_C(A_mod_C)
ans = (1777**A_mod_C) % 10**8
print(ans)
print(len(cached_A_mod_C))
"""