# Question: https://projecteuler.net/problem=243

# This is merely guessing

prod = 1
p = Primes()
next_p = 1
N = 1000
# find a collection of prime numbers to get a small enough R
for i in range(1, N):
    next_p = p.next(next_p)
    prod = prod * next_p
    phi = euler_phi(prod)
    if euler_phi(prod)/(prod-1) < 15499 / 94744:
        break

best = euler_phi(prod)/(prod-1)

# brute force the multiples of prod // next_p
prod = prod // next_p

for i in range(2, N):
    num = prod * i
    phi = euler_phi(num)
    R = euler_phi(num)/(num-1) 
    if R < 15499 / 94744 and R < best: 
        break

print(num)

