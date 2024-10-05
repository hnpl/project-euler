# Question: https://projecteuler.net/problem=45

# The problem asks for T_n = P_m = H_k
#           => n(n+1)/2 = m(3m-1)/2 = k(2k-1)
#           => n(n+1)   = m(3m-1)   = 2k(2k-1)
# Note that 2k(2k-1) and n(n+1) are the products of two adjacent integers, and they are equal when n = 2k-1 (which means n+1 = 2k).
# So, we only need to find m(3m-1) and check whether there exists an integer k such that m(3m-1) = 2k(2k-1).
# To avoid slow floating point operations, we precompute 2k(2k-1) in a certain range.

N = 1000000

hex2 = [2*i*(2*i-1) for i in range(143,N+1)]
hex2_set = set(hex2)

for i in range(166, N+1):
    P_i_2 = i*(3*i-1)
    if P_i_2 in hex2_set:
        break
print(i*(3*i-1)//2)
