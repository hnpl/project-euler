# Question: https://projecteuler.net/problem=29

import numpy as np

N_a = 100
N_b = 100

count = np.ones((N_a+1, N_b+1), dtype = np.uint8)
count[0] = 0
count[1] = 0
count[:,0] = 0
count[:,1] = 0
for x in range(2, 11):
    #  2^{2,4,6,...100} = 4^{1,2,3,...,50}
    # ---
    #  2^{3,6,9,...,99} = 8^{1,2,3,...,33}
    #  4^{3,6,9,...,99} = 8^{2,4,6,...,66}
    # ---
    #  2^{4,8,12,...,100} = 16^{1,2,3,...,25}
    #  4^{4,8,12,...,100} = 16^{2,4,6,...,50}
    #  8^{4,8,12,...,100} = 16^{3,6,9,...,75}
    #  ---
    #  2^{5,10,15,...,100} = 32^{1,2,3,...,20}
    #  4^{5,10,15,...,100} = 32^{2,4,6,...,40}
    #  8^{5,10,15,...,100} = 32^{3,6,9,...,60}
    # 16^{5,10,15,...,100} = 32^{4,8,12,...,80} 
    n = x * x
    k = 2
    while n <= N_a:
        for i in range(1, k):
            #print(x, n, list(range(i, 100//k*i+1, i)))
            count[n][i:100//k*i+1:i] = 0
        n = n * x
        k = k + 1

print(np.sum(count))
""" Doesn't work
start_from = [2] * (N_a+1)

count = 0

for i in range(2, N_a + 1):
    n = i * i
    k = 2
    while n <= N_a:
        start_from[n] = max(start_from[n], N_b // k + 1)
        n = n * i
        k = k + 1
    count = count + N_b - start_from[i] + 1
    print(i, count)

for i, j in enumerate(start_from):
    print(i, start_from[i], end = '| ')
    if i % 20 == 19:
        print()
print(count)
"""
