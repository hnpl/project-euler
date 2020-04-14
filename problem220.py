# Question: https://projecteuler.net/problem=220

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

