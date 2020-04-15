# Question: https://projecteuler.net/problem=79

# Assume that no digit repeats
# Let each digit be a node in a graph. Obviously, this graph must be a DAG. The answer is the topological order of the graph ...

# There'll a node that has no edges that go out, that'll be the target node. The node that has no visiting edge is the first node.

g = {}

has_inward_edges = {} # of the original graph

with open('inputs/p079_keylog.txt', 'r') as f:
    for line in f:
        u = line[0]
        v = line[1]
        w = line[2]
        if not u in g:
            g[u] = {}
            has_inward_edges[u] = False
        if not v in g:
            g[v] = {}
            has_inward_edges[v] = False
        if not w in g:
            g[w] = {}
            has_inward_edges[w] = False
        g[u][v] = 1
        g[v][w] = 1
        has_inward_edges[v] = True
        has_inward_edges[w] = True


visited = set()
vertex = list(g.keys())
reversed_topological_order = []
visiting = { v: False for v in vertex }

def visit(v):
    if v in visited:
        return
    if visiting[v]:
        assert(False)
    visiting[v] = True
    for u in g[v]:
        visit(u)
    visiting[v] = False
    visited.add(v)
    reversed_topological_order.append(v)


N = len(vertex)
# hint the first node
for v in vertex:
    if not has_inward_edges[v]:
        break
visit(v)
while len(visited) < N:
    for v in vertex:
        if not v in visited:
            break
    visit(v)

print(''.join(reversed(reversed_topological_order)))
