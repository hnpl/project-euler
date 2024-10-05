# Question: https://projecteuler.net/problem=265

# Construct a graph where nodes are numbers from 0 to 2**N.
# There're edges from i to j if, the last (N-1) digits of i is the same as the first (N-1) digits of j.
# From the graph, the the sum of Hamiltonian paths starting from 0.

N = 5

def construct_graph(n):
    mask = 2**n - 1
    G = {}
    for i in range(2**n):
        G[i] = {}
        j = (i << 1) & mask
        G[i][j] = 1
        G[i][j+1] = 1
    return G

def path_to_num(path, N):
    n = path[0]
    for e in path[1:-N+1]:
        n = n * 2 + e % 2
    return n

def DFS(G, path, N):
    if len(path) == 2**N:
        mask_N_1 = 2**(N-1) - 1
        if (path[-1] & mask_N_1) >> 1 == path[0] >> 1:
            return path_to_num(path, N)
        return 0
    i = path[-1]
    total = 0
    for j in G[i]:
        if not j in path:
            total = total + DFS(G, path + [j], N)
    return total

G = construct_graph(N)
# start from 0
ans = DFS(G, [0], N)
print(ans)
