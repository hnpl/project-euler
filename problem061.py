# Question: https://projecteuler.net/problem=61

P3n = lambda n: n*(n+1) // 2
P4n = lambda n: n*n
P5n = lambda n: n*(3*n-1)//2
P6n = lambda n: n*(2*n-1)
P7n = lambda n: n*(5*n-3)//2
P8n = lambda n: n*(3*n-2)

has_four_digits = lambda n: n if (n >= 1000) and (n <= 9999) else 0

P3 = {has_four_digits(P3n(n)) for n in range(1, 1000)}
P4 = {has_four_digits(P4n(n)) for n in range(1, 1000)}
P5 = {has_four_digits(P5n(n)) for n in range(1, 1000)}
P6 = {has_four_digits(P6n(n)) for n in range(1, 1000)}
P7 = {has_four_digits(P7n(n)) for n in range(1, 1000)}
P8 = {has_four_digits(P8n(n)) for n in range(1, 1000)}

for P in [P3, P4, P5, P6, P7, P8]:
    P.discard(0)

split = lambda n: (n // 100, n % 100)

G = {}
vertex = set()
G_set = []
P_set = [P3, P4, P5, P6, P7, P8]
W_set = [1, 2, 4, 8, 16, 32]

for P, W in zip(P_set, W_set):
    new_G = {}
    for Pn in P:
        u, v = split(Pn)
        if v < 10:
            continue
        if not u in new_G:
            new_G[u] = {}
        new_G[u][v] = W
    G_set.append(new_G)
"""
# remove impossible combination where the ending of a Pn couldn't be found in the beginning of another Pn of another P set
for i in range(len(G_set)):
    Q = G_set[:i] + G_set[i+1:]
    for u in G_set[i]:
        to_remove = []
        for v in G_set[i][u]:
            found_v = False
            for q in Q:
                if v in q:
                    found_v = True
                    break
            if not found_v:
                to_remove.append(v)
        for v in to_remove:
            del G_set[i][u][v]
"""
for g in G_set:
    to_remove = []
    for u in g:
        if len(g[u]) == 0:
            to_remove.append(u)
    for u in to_remove:
        del g[u]

G = {}

def merge_g(G, H):
    for u in H:
        if not u in G:
            G[u] = {}
        for v, w in H[u].items():
            G[u][v] = w

for g in G_set:
    merge_g(G, g)

# The special cases where u == v are 5151, 5050, 1717, 4141, none of them are in P8
# So it's safe to say that we don't need to consider self loop of a vertex

s = 0

def sum_path(path):
    s = 0
    for i in range(len(path)-1):
        s = s + path[i]*100 + path[i+1]
    s = s + path[-1] * 100 + path[0]
    return s

def DFS(node, path, edges, total_weights, target_node, goal_weight):
    if len(path) == 6:
        if total_weights + 32 == goal_weight and path[-1] == target_node:
            found = True
            global s
            s = sum_path(path)
            return True
        else:
            return False
    u = path[-1]
    found = False
    for v, w in node.items():
        if not (u,v) in edges and total_weights & w == 0 and v in G: # no repeating weights
            if DFS(G[v], path + [v], edges + [(u,v)], total_weights + w, target_node, goal_weight):
                found = True
                break
    return found

for Pn in P8:
    u, v = split(Pn)
    if not v in G:
        continue
    found = DFS(G[v], [v], [], 0, u, sum([1,2,4,8,16,32]))
    if found:
        break

print(s)
