# Question: https://projecteuler.net/problem=813

# - Note that, (8**12) * (12**8) = (2**52) * (3**8).
# - We can treat XOR-multiplication as multiplication of polynomials, in which the coefficent are under GF2 (Z/2Z).
#   Effectively, if the polynomial is written as sum_{i} [c_i * x^a_i], then if c_i = 1, then the bit i of the integer is set.
# - Using the above representation, we have that p = 11 = 0b1011 = x^3 + x^1 + 1.
# - Note that, under GF2, (x+y)^2 = x^2 + y^2. (Ref: https://en.wikipedia.org/wiki/Freshman%27s_dream).
#   So, (sum_{i} [c_i * x^a_i]) ** (2**52) = sum_{i} [c_i * x^(a_i * (2**52))]
# - Therefore, we only need to compute p3 = p**(3**8) first, then we can multiply the exponentsof non-zero coefficients by 2**52.
# - SageMath has good support for polynomial under GF2 so we use SageMath.
# - More explaination: https://math.stackexchange.com/questions/4628358/xor-product-modulo-prime

from sage.all import *

N = (8**12) * (12**8)
MOD = 10**9+7

P = PolynomialRing(IntegerModRing(2), 't')
print(type(P))
t = P.gen()
p = t**3 + t + 1

factored_N = {}
for base, exp in list(factor(N)):
    factored_N[base] = exp
N_base_2_exp = factored_N[2]
N_base_3_exp = factored_N[3]

# construct p3 = p ** (3 ** N_base_3_exp)
p3 = p ** (3 ** N_base_3_exp)
p3_exp = [exp for exp in p3.dict().keys()]

# construct p2 = p3 ** (2 ** N_base_2_exp)
p2_exp = [exp * (2**52) for exp in p3_exp]

p2_val = [pow(2, exp, MOD) for exp in p2_exp]

ans = sum(p2_val) % MOD
print(ans)
