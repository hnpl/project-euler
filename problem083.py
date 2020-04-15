# Question: https://projecteuler.net/problem=83

import numpy as np

N = 80

index = lambda i, j: i*N + j

G = {}
for i in range(N*N):
    G[i] = {}


cost_table = np.zeros((N, N), dtype = np.uint64)

with open("inputs/p083_matrix.txt", "r") as f:
    for i, line in enumerate(f):
        for j, cost in enumerate(list(map(int, line.strip().split(",")))):
            cost_table[i][j] = cost

# build the graph
for i in range(N):
    for j in range(N):
        if i < N - 1:
            G[index(i, j)][index(i+1, j)] = cost_table[i+1][j]
            G[index(i+1, j)][index(i, j)] = cost_table[i][j]
        if j < N - 1:
            G[index(i, j)][index(i, j+1)] = cost_table[i][j+1]
            G[index(i, j+1)][index(i, j)] = cost_table[i][j]

source = 0
target = N*N - 1
INF = 10**12
dist = [INF] * (N*N)
vertex = set(range(N*N))

# Dijkstra's algorithm (without priority queue)
for v in vertex:
    dist[v] = INF
dist[source] = 0
while target in vertex:
    min_v = -1
    min_dist = INF
    for v in vertex:
        if dist[v] < min_dist:
            min_dist = dist[v]
            min_v = v
    u = min_v
    vertex.discard(u)
    for v in G[u].keys():
        dist[v] = min(dist[v], dist[u] + G[u][v])

print(dist[target] + cost_table[0][0])


