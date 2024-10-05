# Question: https://projecteuler.net/problem=122

import numpy as np

N = 200
INF = 0xFFFFFFFFFFFFFFFFFFFF

optimal = np.zeros(N+1, dtype = np.uint64) + INF
optimal[0] = 0
optimal[1] = 0

def DFS(n, depth, cost, have):
    global N
    #print(depth, n, cost, have)
    if n > N:
        return
    if cost > 12: # this is a heuristic
        return
    if cost <= optimal[n]:
        optimal[n] = cost
    else:
        return
    s = reversed(sorted(list(have)))
    for add in s:
        if n + add > N:
            continue
        DFS(n + add, depth + 1, cost + 1, have | {n + add})

DFS(1, 1, 0, {1})

print(np.sum(optimal[1:]))
