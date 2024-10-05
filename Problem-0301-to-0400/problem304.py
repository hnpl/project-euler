# Question: https://projecteuler.net/problem=304

x = next_prime(10**14)
MOD = 1234567891011
R = IntegerModRing(MOD)
r = R(0)

def fib(x, prevA, prev_x):
    A = prevA * (matrix(R, [[1, 1], [1, 0]]) ^ (x-prev_x))
    B = matrix(R, [[1],[0]])
    C = A * B
    return C[0][0], A

A = matrix(R, [[1, 1], [1, 0]])
prev_x = 2

for i in range(100000):
    b, A = fib(x, A, prev_x)
    r = r + b
    prev_x, x = x, next_prime(x)

print(r)

#def fib(x):
#    A = matrix(R, [[1, 1], [1, 0]])
#    B = matrix(R, [[1],[0]])
#    C = (A ^ (x-1)) * B 
#    return C[0][0]
#
#assert(fib(2) == 1)
#assert(fib(3) == 2)
#
#for i in range(100000):
#    b = fib(a)
#    r = r + b 
#    a = next_prime(a)
#    break
#
#print(r)