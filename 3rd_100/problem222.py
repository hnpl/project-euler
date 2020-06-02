# Question: https://projecteuler.net/problem=222

# Distance of 2 spheres packed next to each other:
#    delta_y = 100 - r1 - r2
#    delta_x = sqrt((r1+r2)^2 - delta_y^2)
#            = sqrt(200r1 + 200r2 - 10000)
# The total width of the packed n spheres is
#    w = r_1 + sum_{i \in [2, n]} (sqrt(200r_{i-1} + 200r_i - 10000)) + r_n
# Obviously, since we want to minimize w, either r_1 and r_n must contain the biggest radius.
# Why?
# Suppose r_max is not in either end, then we can swap r_max with r_i in one of the ends we have a smaller w.
# The same can be said for the second biggest r_i.
# TODO: prove the above claims.

from sage.all import *

Y = 100.0
g = graphs.EmptyGraph()
#SOURCE = 0
#TARGET = 1
R_MIN = 30
R_MAX = 50

def delta_x(r1, r2, Y):
    delta_y = 100.0 - r1 - r2
    ans = sqrt((r1 + r2)**2 - delta_y**2)
    return ans

for u in range(R_MIN, R_MAX+1):
    for v in range(R_MIN, R_MAX+1):
        if u == v:
            continue
        g.add_edge(u, v, delta_x(u, v, Y))

#for u in range(R_MIN, R_MAX+1):
    #g.add_edge(SOURCE, u, u)
    #g.add_edge(u, TARGET, u)

subgraph = g.hamiltonian_path(s = R_MAX, t = R_MAX-1, use_edge_labels = True, verbose = 10)
ans = subgraph[0]
print(ans + R_MAX + R_MAX - 1)
#print(subgraph[1].all_paths(R_MAX, R_MAX-1)[0])

