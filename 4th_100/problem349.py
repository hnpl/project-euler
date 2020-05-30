# Question: https://projecteuler.net/problem=349

# https://en.wikipedia.org/wiki/Langton%27s_ant
# -> after a while, the ant follows a periodic sequence of moves of length 104

import numpy as np

class Direction:
    NORTH, EAST, SOUTH, WEST = 0, 1, 2, 3
    _size = 4

class Color:
    BLACK, WHITE = 1, 0

class Ant:
    def __init__(self, grid_controller, initial_direction, initial_x, initial_y, cycle_length):
        self.grid_controller = grid_controller
        self.direction = initial_direction
        self.x = initial_x
        self.y = initial_y
        self.move_history = 0
        self.mask = 2**(4*cycle_length)-1 # 2 bits per move, store 2*cycle_length to detect the cycle
        self.lowerhalf_mask = 2**(2*cycle_length) - 1
        self.upperhalf_mask = self.mask ^ self.lowerhalf_mask
        self.cycle_length = cycle_length
        self.n_moves = 0
    def rotate_clockwise(self):
        self.direction = self.direction + 1
        self.direction = self.direction % Direction._size
    def rotate_counter_clockwise(self):
        self.direction = self.direction - 1
        self.direction = self.direction % Direction._size
    def move_forward(self):
        if self.direction == Direction.NORTH:
            self.y = self.y - 1
        elif self.direction == Direction.EAST:
            self.x = self.x + 1
        elif self.direction == Direction.SOUTH:
            self.y = self.y + 1
        elif self.direction == Direction.WEST:
            self.x = self.x - 1
    def move(self):
        self.n_moves = self.n_moves + 1
        if self.grid_controller.get_tile_color(self.x , self.y) == Color.BLACK:
            self.grid_controller.flip_tile_color(self.x, self.y)
            self.rotate_counter_clockwise()
            self.move_forward()
            self.add_history(self.direction)
        elif self.grid_controller.get_tile_color(self.x, self.y) == Color.WHITE:
            self.grid_controller.flip_tile_color(self.x, self.y)
            self.rotate_clockwise()
            self.move_forward()
            self.add_history(self.direction)
    def add_history(self, direction):
        self.move_history = (self.move_history << 2) + direction
        self.move_history = self.move_history & self.mask
    def check_move_history(self):
        upper_half = (self.move_history & self.upperhalf_mask) >> (2*self.cycle_length)
        lower_half = self.move_history & self.lowerhalf_mask
        return upper_half == lower_half

class GridController:
    def get_tile_color(self, x, y):
        pass
    def flip_tile_color(self, x, y):
        pass
    def get_number_of_black_tiles(self):
        pass
    def get_number_of_white_tiles(self):
        pass

class Grid(GridController):
    def __init__(self, w, h):
        self.w = w
        self.h = h
        self.grid = np.zeros((w, h), dtype = np.bool) + Color.WHITE
    def get_tile_color(self, x, y):
        return self.grid[x, y]
    def flip_tile_color(self, x, y):
        self.grid[x][y] = not (self.grid[x][y])
    def get_number_of_black_tiles(self):
        return np.where(self.grid == Color.BLACK)[0].shape[0]
    def get_number_of_white_tiles(self):
        return self.w * self.h - self.get_number_of_black_tiles()

ans = 0
W = 300
H = 300
cycle_length = 104
N = 10**18

grid = Grid(W, H)
ant = Ant(grid, Direction.NORTH, W//2, H//2, cycle_length)
for step in range(20000):
    ant.move()
    if ant.check_move_history():
        break
n_black_tiles = grid.get_number_of_black_tiles()
delta_black_tiles = [0] * (cycle_length + 1) 
for i in range(1, cycle_length+1):
    ant.move()
    delta_black_tiles[i] = grid.get_number_of_black_tiles() - n_black_tiles
delta_black_tiles[0] = delta_black_tiles[-1]
ans = grid.get_number_of_black_tiles() + ((N - ant.n_moves) // cycle_length) * delta_black_tiles[cycle_length] + delta_black_tiles[(N - ant.n_moves) % cycle_length]
print(ans)
