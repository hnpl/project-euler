# Problem: https://projecteuler.net/problem=324

from sage.all import *

N = 10**10000
MOD = 100000007

WIDTH = 3
HEIGHT = 3

def mask(n):
    return 2**n - 1

class Shape:
    def __init__(self, label):
        self.__width = WIDTH+1
        self.hash = 0
        self.most_significant_one_index = 0
        self.label = label
        self.n_occupied_tiles = 0
        self.occupied_tiles = []

    def add_tile(self, row, col):
        self.hash = self.hash | (1 << (row * self.__width + col))
        self.most_significant_one_index = max(self.most_significant_one_index, (row * self.__width + col))
        self.n_occupied_tiles = self.n_occupied_tiles + 1
        self.occupied_tiles.append((row, col))

    def get_hash(self):
        return self.hash

    def get_possible_shapes():
        shapes = []

        shape1 = Shape(0)
        shape1.add_tile(0, 0)
        shape1.add_tile(0, 1)
        shapes.append(shape1)

        shape2 = Shape(0)
        shape2.add_tile(0, 0)
        shape2.add_tile(1, 0)
        shapes.append(shape2)

        shape3 = Shape(1)
        shape3.add_tile(0, 0)
        shapes.append(shape3)
        return shapes

class State:
    __width = WIDTH+1
    __height = HEIGHT
    
    def __init__(self):
        self.state_hash = 0 
        self.occupy_hash = State.__get_initial_occupy_hash() # occupy_hash with fences placed
        self.n_empty_tiles = (State.__width-1) * State.__height

    # Private -----------------------------------------------------------------
    def __get_initial_occupy_hash():
        state_hash = 0
        for row in range(State.__height):
            state_hash = state_hash | (1 << (row * State.__width + State.__width - 1))
        return state_hash

    def __init_from_hashes(self, state_hash, occupy_hash):
        self.state_hash = state_hash
        self.occupy_hash = occupy_hash
        self.n_empty_tiles = self.__count_n_empty_tiles()
        return self

    def __count_n_empty_tiles(self):
        occupy_hash = self.occupy_hash
        n_empty_tiles = 0
        while occupy_hash > 0:
            if occupy_hash & mask(1) == 0:
                n_empty_tiles = n_empty_tiles + 1
            occupy_hash >>= 1
        return n_empty_tiles

    def __try_placing_shape(self, shape, row, col):
        is_valid = True
        new_state_hash = self.state_hash
        new_occupy_hash = self.occupy_hash

        shift_factor = row * State.__width + col
        shape_hash = shape.hash
        shifted_shape_hash = shape_hash << shift_factor

        # check if the shape is placed outside of the grid
        if shift_factor + shape.most_significant_one_index >= State.__width * State.__height:
            is_valid = False

        # check if the shape occupies the occupied tiles
        if is_valid and (self.occupy_hash & shifted_shape_hash) == 0:
            shape_label = shape.label
            for occupied_tile_row, occupied_tile_col in shape.occupied_tiles:
                new_occupied_tile_row = occupied_tile_row + row
                new_occupied_tile_col = occupied_tile_col + col
                new_state_hash = new_state_hash | (shape_label << (new_occupied_tile_row * State.__width + new_occupied_tile_col))
            new_occupy_hash = self.occupy_hash | shifted_shape_hash
        else:
            is_valid = False
        return is_valid, new_state_hash, new_occupy_hash

    # Public ------------------------------------------------------------------
    def is_full(self):
        return self.n_empty_tiles == 0

    def get_hash(self):
        return (self.occupy_hash << (State.__width * State.__height)) | self.state_hash

    def add_tile(self, row, col, label): # add a single tile, i.e. shape3
        self.state_hash = self.state_hash | (label << (row * State.__width + col))
        self.occupy_hash = self.occupy_hash | (1 << (row * State.__width + col))
        self.n_empty_tiles = self.n_empty_tiles - 1

    def get_state_hash(self): # get state_hash with fences removed
        final_state_hash = 0
        state_hash = self.state_hash

        final_state_hash_msb_index = 0
        for row in range(State.__height):
            final_state_hash = final_state_hash | ((state_hash & mask(State.__width - 1)) << final_state_hash_msb_index)
            state_hash = state_hash >> State.__width
            final_state_hash_msb_index += State.__width - 1
        return final_state_hash

    def generate_next_states(self, possible_shapes):
        next_states = []

        # find the first empty cell
        occupy_hash = self.occupy_hash
        wh = State.__width * State.__height
        found_empty_tile = False
        for pos in range(wh):
            if (occupy_hash & mask(1)) == 0:
                found_empty_tile = True
                break
            occupy_hash >>= 1

        if found_empty_tile:
            row = pos // State.__width
            col = pos % State.__width
            for shape in possible_shapes:
                is_valid, new_state_hash, new_occupy_hash = self.__try_placing_shape(shape, row, col)
                if is_valid:
                    new_state = State().__init_from_hashes(new_state_hash, new_occupy_hash)
                    next_states.append(new_state)

        return next_states

    def describe_state(self):
        print("State", bin(self.state_hash), " Occupying ", bin(self.occupy_hash))

cache = {}

def dfs(state, possible_shapes):
    print("exploring ", end = '')
    state.describe_state()

    if state.is_full():
        print("full ", end = '')
        state.describe_state()
        return 1
    
    if state.get_hash() in cache:
        return cache[state.get_hash()]

    state_count = 0

    for next_state in state.generate_next_states(possible_shapes):
        state_count += dfs(next_state, possible_shapes)

    if not state.get_hash() in cache:
        cache[state.get_hash()] = state_count

    return state_count

def count_state_permutations(inital_state, possible_shapes):
    return dfs(inital_state, possible_shapes)

if __name__ == "__main__":
    ans = 0

    shapes = Shape.get_possible_shapes()

    inital_state = State()

    print(count_state_permutations(inital_state, shapes))

    print(ans)