# Question: https://projecteuler.net/problem=220

# I. REPRESENTATION
#
# a -> (aRb)FR -> (aRb)FRRLF(aLb)FR
# (aRb) -> (aRb)FRRLF(aLb) = (aRb)FRF(aLb)
# (aLb) -> (aRb)FRLLF(aLb) = (aRb)FLF(aLb)
# So, (aRb) and (aLb) generate strings with patterns of (aRb)ccc(aLb) where ccc does not regenerate.
#
# Let 1 <- aRb (0 steps)
#     2 <- FRF (2 steps)
#     3 <- aLb (0 steps)
#     4 <- FLF (2 steps)
# So, 1 -> 123 and 3 -> 143
# Then, D[1] = 1
#       D[2] = 123
#       D[3] = 123 2 143
#       D[4] = 123 2 143 2 123 4 143
# This is the inorder traversal pattern of a perfect binary tree, of which the root is '2', the left internal nodes are '2', the right internal nodes are '4', the left leaves are '1' and the right leaves are '3'.
#
# After 1 iteration, the '1' leaves become subtrees of the shape
#         2
#       /   \
#       1   3
# and the '3' leaves become subtrees of the shape
#         4
#       /   \
#       1   3
# while other nodes are the same.
# 
# Let L[n] denote the perfect binary tree of the above properties with root is '2',
#     R[n] denote the perfect binary tree of the above properties with root is '4'.
# The inorder traversal pattern of L[n] is that, to traverse L[n], we traverse left node of L[n], root of L[n], right node of L[n].
# Note that left node of L[n] is L[n-1], root of L[n] is '2', right node of L[n] is R[n-1].
#             L[n]:       2                              R[n]:            4
#                        / \                                             / \
#                   L[n-1] R[n-1]                                   L[n-1] R[n-1]
# Basically, traverse(L[n]) = traverse(L[n-1]) + '2' + traverse(R[n-1])
#            traverse(R[n]) = traverse(L[n-1]) + '4' + traverse(R[n-1])
# II. CALCULATING THE DISPLACEMENT
# Note that, we only care about the displacement of the cursor, and we don't care about the path, so we only need to keep track of delta_x, delta_y, and delta_direction of the cursor for each subtree of the tree, and we can use memoization to avoid recalculating the displacement.
# ie,
# displacement = (delta_x, delta_y, delta_direction)
# displacement(L[n]) = displacement(L[n-1]) + displacement('2') + displacement(R[n-1])
# displacement(R[n]) = displacement(L[n-1]) + displacement('4') + displacement(R[n-1])
# where displacement_1 + displacement_2 = (delta_x1 + delta_x2, delta_y1 + delta_y2, (delta_direction_1 + delta_direction_2) % 5)
#
# III. TRAVERSAL ORDER
# Note that, D_50 has ~2^50 nodes >> 10^12, the cursor will not traverse the whole tree.
# Also, inorder traverse prioritizes traversing the left subtree then root then right subtree.
# So, the traversal pattern would actually be L[x_1] -> parent of L[x_1] -> L[x_2] -> parent of L[x_2] -> L[x_3] -> parent of L[x_3] -> ...
#                                                                           ^^^^^^^^^^^^^^^^^^^^^^^^^^
#                                                                           sub-sub-sub-...-subtree of R[x_1] (we might not traverse the whole R[x_1])
# where x_1 > x_2 > x_3 > ...
#
#                                                 L[x_1+1]
#                                                /        \
#                                             L[x_1]    .....
#                                                      /
#                                                  L[x_2+1]
#                                                 /        \
#                                              L[x_2]     .....
#                                                        /
#                                                     L[x_3]
# 
# Suppose we know the number of steps in L[i] for all i, and the number of steps is n_steps.
# Then, first, we find L[x_1] such that steps[L[x_1]] < n_steps < steps[L[x_1+1]]
#           -> n_steps - steps[L[x_1]] steps remaining
#       then, we traverse the root of L[x_1+1]
#           -> n_steps - steps[L[x_1]] - 2 steps remaining
#       then, we find L[x_2] such that steps[L[x_2]] < n_steps_remaining < steps[L[x_2+1]]
#           -> n_steps - steps[L[x_1]] - 2 - steps[L[x_2]] steps remaining
#       then, we traverse the root of L[x_2+1]
#           -> n_steps - steps[L[x_1]] - 2 - steps[L[x_2]] - 2 steps remaining
# and so on ...
#
# Suppose we know x_1, x_2, x_3, ...
# What are the parent of x_1, x_2, x_3, ... ? (or, which ones are '2', and which ones are '4' ?)
# Obviously, root(L[x_1+1]) is '2' (first, we need to traverse the whole left subtree of a node).
# There are two cases,
#     Case 1: x_i = x_j + 1 -> root(R[x_i]) is the parent of L[x_j]
#                     L[x_i+1]
#                    /        \
#                 L[x_i]    R[x_i]
#                          /      \
#                      L[x_i-1]  .....
#                      ^^^^^^^^
#                       L[x_j]
#             -> parent of L[x_j] is '4'
#     Case 2: x_i > x_j + 1
#                     L[x_i+1]
#                    /        \ 
#                 L[x_i]    R[x_i]
#                          /
#                      L[x_i-1]
#                     /
#                   .....
#                   /
#                L[x_j+1]
#                 /
#              L[x_j]
#              -> parent of L[x_j] is '2'


n_steps = 10**12
N = 50



# 1 -> R
# 2 -> FRF
# 3 -> L
# 4 -> FLF

tree_steps = [0] * (N+1)
tree_leaves = [0] * (N+1)
tree_internal_nodes = [0] * (N+1)

tree_steps[0] = 1
tree_leaves[1] = 1
tree_internal_nodes[1] = 0
tree_steps[1] = 0

for h in range(2, N+1):
    tree_leaves[h] = 2 ** (h-1)
    tree_internal_nodes[h] = 2**h - 1 - tree_leaves[h]
    tree_steps[h] = 0 * tree_leaves[h] + 2 * tree_internal_nodes[h]


# ignore the first F
n_remaining_steps = n_steps - 1
traversal_order = []
traversal_order_readable = []

while n_remaining_steps > 0:
    h = 0
    # left
    while tree_steps[h+1] < n_remaining_steps:
        h = h + 1
    traversal_order.append(h)
    traversal_order_readable.append('L{}'.format(h))
    n_remaining_steps = n_remaining_steps - tree_steps[h]

    # mid
    if n_remaining_steps > 0:
        n_remaining_steps = n_remaining_steps - 2
        if len(traversal_order) < 2:
            traversal_order.append(-2) # left node of its parent
            traversal_order_readable.append('2')
        else:
            if traversal_order[-3] - traversal_order[-1] > 1:
                traversal_order.append(-2) # left node of its parent
                traversal_order_readable.append('2')
            else:
                traversal_order.append(-4) # right node of its parent
                traversal_order_readable.append('4')

class DIRECTIONS:
    NORTH, EAST, SOUTH, WEST = 0, 1, 2, 3

class MoveDelta:
    DELTA_COORS = { DIRECTIONS.NORTH: {'x':  0, 'y':  1},
                    DIRECTIONS.EAST:  {'x':  1, 'y':  0},
                    DIRECTIONS.SOUTH: {'x':  0, 'y': -1},
                    DIRECTIONS.WEST:  {'x': -1, 'y':  0} }
    
    def __init__(self, delta_x = 0, delta_y = 0, delta_direction = 0):
        self.delta_x = delta_x
        self.delta_y = delta_y
        self.delta_direction = delta_direction % 4

    def move_forward(self):
        delta_coors = self.DELTA_COORS[self.delta_direction]
        self.delta_x = self.delta_x + delta_coors['x']
        self.delta_y = self.delta_y + delta_coors['y']

    def turn_left(self):
        self.delta_direction = (self.delta_direction - 1) % 4

    def turn_right(self):
        self.delta_direction = (self.delta_direction + 1) % 4

    def add(self, initial_x, initial_y, initial_direction):
        delta_x = self.delta_x
        delta_y = self.delta_y
        for i in range(initial_direction):
            delta_x, delta_y = delta_y, -delta_x
        x = initial_x + delta_x
        y = initial_y + delta_y
        direction = (initial_direction + self.delta_direction) % 4
        return x, y, direction

    def move(self, seq):
        for m in seq:
            if m == 'F':
                self.move_forward()
            elif m == 'L':
                self.turn_left()
            elif m == 'R':
                self.turn_right()


max_h = max(traversal_order)

delta_L = [None] * (N+1)
delta_R = [None] * (N+1)

move_1_delta = MoveDelta()
move_1_delta.turn_right()

move_2_delta = MoveDelta()
move_2_delta.move('FRF')

move_3_delta = MoveDelta()
move_3_delta.turn_left()

move_4_delta = MoveDelta()
move_4_delta.move('FLF')

delta_L[1] = move_1_delta
delta_R[1] = move_3_delta

for i in range(2, max_h+1):
    # left node
    delta_x = 0
    delta_y = 0
    delta_direction = 0
    delta_x, delta_y, delta_direction = delta_L[i-1].add(delta_x, delta_y, delta_direction)
    delta_x, delta_y, delta_direction = move_2_delta.add(delta_x, delta_y, delta_direction)
    delta_x, delta_y, delta_direction = delta_R[i-1].add(delta_x, delta_y, delta_direction)
    delta_L[i] = MoveDelta(delta_x, delta_y, delta_direction)

    # right node
    delta_x = 0
    delta_y = 0
    delta_direction = 0
    delta_x, delta_y, delta_direction = delta_L[i-1].add(delta_x, delta_y, delta_direction)
    delta_x, delta_y, delta_direction = move_4_delta.add(delta_x, delta_y, delta_direction)
    delta_x, delta_y, delta_direction = delta_R[i-1].add(delta_x, delta_y, delta_direction)
    delta_R[i] = MoveDelta(delta_x, delta_y, delta_direction)

# The first step is 'F'
# This is position after the first step    
x = 0
y = 1
direction = DIRECTIONS.NORTH

for m in traversal_order[:-1]:
    delta_move = None
    if m > 0: # traverse all nodes of the left of m + 1
        delta_move = delta_L[m]
    elif m == -4: # move 4
        delta_move = move_4_delta
    elif m == -2: # move 2
        delta_move = move_2_delta
    x, y, direction = delta_move.add(x, y, direction)

move_2_seq = 'FRF'
move_4_seq = 'FLF'

delta_move = None
m = traversal_order[-1]

if m >= 0:
    if not m == 0: # m = 0 -> right node
        delta_move = delta_L[m]
    else:
        delta_move = move_3_delta
elif m == -2:
    delta_move = MoveDelta()
    if n_remaining_steps == 0:
        delta_move.move(move_2_seq)
    elif n_remaining_steps == -1:
        delta_move.move_forward()
elif m == -4:
    delta_move = MoveDelta()
    if n_remaining_steps == 0:
        delta_move.move(move_4_seq)
    elif n_remaining_steps == -1:
        delta_move.move_forward()

x, y, _ = delta_move.add(x, y, direction)

print("{},{}".format(x, y))

