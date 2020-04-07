# Question: https://projecteuler.net/problem=46

p = Primes()

N = 100000

squares = [i*i for i in range(N)]

comp_odd = 9

while True:
    stop = True
    for square in squares:
        if (comp_odd - 2*square) in p:
            stop = False
            break
    if stop:
        break
    comp_odd = comp_odd + 2
    while comp_odd in p:
        comp_odd = comp_odd + 2

print(comp_odd)
