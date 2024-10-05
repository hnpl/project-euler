# Question: https://projecteuler.net/problem=107

from collections import defaultdict
from queue import PriorityQueue

g = defaultdict(dict)

edges_pq = PriorityQueue()

total_w = 0

with open("inputs/p107_network.txt", "r") as f:
    for u, line in enumerate(f):
        for v, weight in enumerate(line.strip().split(',')):
            if weight == '-':
                continue
            g[u][v] = int(weight)
            if u <= v:
                edges_pq.put((g[u][v], (u, v)))
                total_w = total_w + g[u][v]

# Prim's algorithm for finding minimal spanning tree
# MST = set()
MST_weight = 0
vertex = set(g.keys())
v_set_map = {v: v for v in vertex} # v_set_map[v] = x -> v is in set x
n_sets = len(v_set_map)
while not edges_pq.empty() and n_sets > 1:
    w, (u, v) = edges_pq.get()
    set_u = v_set_map[u]
    set_v = v_set_map[v]
    if not set_u == set_v:
        # MST.add((u, v))
        MST_weight = MST_weight + w
        for vtx in v_set_map:
            if v_set_map[vtx] == set_v:
                v_set_map[vtx] = set_u
        n_sets = n_sets - 1

print(total_w - MST_weight)
