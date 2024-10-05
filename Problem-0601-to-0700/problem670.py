import numpy as np

N = 10**16
MOD = 10**9 + 4321

COLOR_BIT_WIDTH = 2
TILE_BIT_WIDTH = 2

class Tile:
    TILE_1_1 = 0
    TILE_1_2 = 1
    TILE_1_3 = 2
    TILE_2_1 = 3
    N_TILES = 4

COLOR_SET = { 0, 1, 2, 3 }

class State:
    def __init__(self, row1_most_recent_color,
                       row1_leftmost_tile,
                       row2_most_recent_color, 
                       row2_leftmost_tile):
        self.row1_most_recent_color = row1_most_recent_color
        self.row1_leftmost_tile = row1_leftmost_tile
        self.row2_most_recent_color = row2_most_recent_color
        self.row2_leftmost_tile = row2_leftmost_tile
        self.hash = State.encode(row1_most_recent_color,
                                 row1_leftmost_tile,
                                 row2_most_recent_color,
                                 row2_leftmost_tile)

    def get_hash(self):
        return self.hash

    def encode(row1_most_recent_color,
               row1_leftmost_tile,
               row2_most_recent_color,
               row2_leftmost_tile):
        h = 0
        msb = 0
        
        h |= (row1_most_recent_color << msb)
        msb += COLOR_BIT_WIDTH
        h |= (row1_leftmost_tile << msb)
        msb += TILE_BIT_WIDTH
        h |= (row2_most_recent_color << msb)
        msb += COLOR_BIT_WIDTH
        h |= (row2_leftmost_tile << msb)
        msb += TILE_BIT_WIDTH
        
        return h

    def decode(hash):
        row1_most_recent_color = hash & get_mask(COLOR_BIT_WIDTH)
        hash >>= COLOR_BIT_WIDTH
        row1_leftmost_tile = hash & get_mask(TILE_BIT_WIDTH)
        hash >>= TILE_BIT_WIDTH
        row2_most_recent_color = hash & get_mask(COLOR_BIT_WIDTH)
        hash >>= COLOR_BIT_WIDTH
        row2_leftmost_tile = hash & get_mask(TILE_BIT_WIDTH)
        hash >>= TILE_BIT_WIDTH
        return row1_most_recent_color, row1_leftmost_tile, row2_most_recent_color, row2_leftmost_tile

    def get_state_info(self):
        return self.row1_most_recent_color, self.row1_leftmost_tile, self.row2_most_recent_color, self.row2_leftmost_tile

    def get_initial_states():
        initial_states = []
        row1_leftmost_tile = row2_leftmost_tile = Tile.TILE_1_1
        color_set = set(COLOR_SET)
        for row1_color in color_set:
            for row2_color in color_set:
                if row1_color == row2_color:
                    continue
                initial_states.append(State(row1_color,
                                            row1_leftmost_tile,
                                            row2_color,
                                            row2_leftmost_tile))
        for color in color_set:
            initial_states.append(State(color, Tile.TILE_2_1, color, Tile.TILE_2_1))
        return initial_states

    def get_next_states(self):
        next_states = []

        # extend the tile on the first row, place 1x1 on the second row
        if self.row1_leftmost_tile != Tile.TILE_1_3 and self.row1_leftmost_tile != Tile.TILE_2_1:
            new_row1_most_recent_color, new_row1_leftmost_tile, new_row2_most_recent_color, new_row2_leftmost_tile = self.get_state_info()
 
            if self.row1_leftmost_tile == Tile.TILE_1_1:
                new_row1_leftmost_tile = Tile.TILE_1_2
            elif self.row1_leftmost_tile == Tile.TILE_1_2:
                new_row1_leftmost_tile = Tile.TILE_1_3
            new_row2_leftmost_tile = Tile.TILE_1_1
            
            row2_color_set = set(COLOR_SET)
            row2_color_set -= { new_row1_most_recent_color, self.row2_most_recent_color }
            
            for row2_color in row2_color_set:
                next_states.append(State(new_row1_most_recent_color,
                                         new_row1_leftmost_tile,
                                         row2_color,
                                         new_row2_leftmost_tile))

        # place 1x1 on the first row, extend the tile on the second row
        if self.row2_leftmost_tile != Tile.TILE_1_3 and self.row2_leftmost_tile != Tile.TILE_2_1:
            new_row1_most_recent_color, new_row1_leftmost_tile, new_row2_most_recent_color, new_row2_leftmost_tile = self.get_state_info()
            
            if self.row2_leftmost_tile == Tile.TILE_1_1:
                new_row2_leftmost_tile = Tile.TILE_1_2
            elif self.row2_leftmost_tile == Tile.TILE_1_2:
                new_row2_leftmost_tile = Tile.TILE_1_3
            new_row1_leftmost_tile = Tile.TILE_1_1
            
            row1_color_set = set(COLOR_SET)
            row1_color_set -= { new_row2_most_recent_color, self.row1_most_recent_color }
            
            for row1_color in row1_color_set:
                next_states.append(State(row1_color,
                                         new_row1_leftmost_tile,
                                         new_row2_most_recent_color,
                                         new_row2_leftmost_tile))

        # extend tiles on both rows
        if self.row1_leftmost_tile != Tile.TILE_1_3 and self.row1_leftmost_tile != Tile.TILE_2_1 and self.row2_leftmost_tile != Tile.TILE_1_3 and self.row2_leftmost_tile != Tile.TILE_2_1:
            new_row1_most_recent_color, new_row1_leftmost_tile, new_row2_most_recent_color, new_row2_leftmost_tile = self.get_state_info()

            if self.row1_leftmost_tile == Tile.TILE_1_1:
                new_row1_leftmost_tile = Tile.TILE_1_2
            elif self.row1_leftmost_tile == Tile.TILE_1_2:
                new_row1_leftmost_tile = Tile.TILE_1_3

            if self.row2_leftmost_tile == Tile.TILE_1_1:
                new_row2_leftmost_tile = Tile.TILE_1_2
            elif self.row2_leftmost_tile == Tile.TILE_1_2:
                new_row2_leftmost_tile = Tile.TILE_1_3

            next_states.append(State(new_row1_most_recent_color,
                                     new_row1_leftmost_tile,
                                     new_row2_most_recent_color,
                                     new_row2_leftmost_tile))

        # place 1x1 on both rows
        if self.row1_leftmost_tile == Tile.TILE_2_1:
            new_row1_most_recent_color, new_row1_leftmost_tile, new_row2_most_recent_color, new_row2_leftmost_tile = self.get_state_info()
            new_row1_leftmost_tile = Tile.TILE_1_1
            new_row2_leftmost_tile = Tile.TILE_1_1
            color_set = set(COLOR_SET)
            color_set -= {self.row1_most_recent_color}
            for row1_color in color_set:
                for row2_color in color_set:
                    if row1_color == row2_color:
                        continue
                    new_row1_most_recent_color = row1_color
                    new_row2_most_recent_color = row2_color
                    next_states.append(State(new_row1_most_recent_color,
                                             new_row1_leftmost_tile,
                                             new_row2_most_recent_color,
                                             new_row2_leftmost_tile))
        # place a 2x1 on both rows
        new_row1_most_recent_color, new_row1_leftmost_tile, new_row2_most_recent_color, new_row2_leftmost_tile = self.get_state_info()
        new_row1_leftmost_tile = Tile.TILE_2_1
        new_row2_leftmost_tile = Tile.TILE_2_1
        color_set = set(COLOR_SET)
        color_set -= { self.row1_most_recent_color, self.row2_most_recent_color }
        for color in color_set:
            new_row1_most_recent_color = color
            new_row2_most_recent_color = color
            next_states.append(State(new_row1_most_recent_color,
                                     new_row1_leftmost_tile,
                                     new_row2_most_recent_color,
                                     new_row2_leftmost_tile))

        return next_states

    def tile_to_string(n):
        return ["1x1", "1x2", "1x3", "2x1"][n]

    def to_string(self):
        return f"({self.row1_most_recent_color}, {State.tile_to_string(self.row1_leftmost_tile)}, {self.row2_most_recent_color}, {State.tile_to_string(self.row2_leftmost_tile)})"

def pow_mod(a, exp, mod):
    n, _ = a.shape

    base = np.array(a, dtype = object)
    ans = np.eye(n, dtype = object)

    while exp > 0:
        if exp % 2 == 1:
            ans = np.matmul(ans, base)
            ans = np.mod(ans, mod)
        exp //= 2
        base = np.matmul(base, base)
        base = np.mod(base, mod)

    return ans

def discover_states():
    hash_state_map = {}
    index_hash_map = []
    hash_index_map = {}

    color_set = set(COLOR_SET)
    tile_set = { Tile.TILE_1_1, Tile.TILE_1_2, Tile.TILE_1_3, Tile.TILE_2_1 }
    for row1_tile in tile_set:
        for row2_tile in tile_set:
            if row1_tile == Tile.TILE_2_1 or row2_tile == Tile.TILE_2_1:
                if not (row1_tile == Tile.TILE_2_1 and row2_tile == Tile.TILE_2_1):
                    continue
            for row1_color in color_set:
                for row2_color in color_set:
                    if row1_tile == row2_tile and row1_tile == Tile.TILE_2_1:
                        if not (row1_color == row2_color):
                            continue
                    state = State(row1_color, row1_tile, row2_color, row2_tile)
                    state_hash = state.get_hash()
                    hash_state_map[state_hash] = state
                    index_hash_map.append(state_hash)
                    hash_index_map[state_hash] = len(index_hash_map) - 1

    return hash_state_map, index_hash_map, hash_index_map



def solve(n):
    ans = 0

    hash_state_map, index_hash_map, hash_index_map = discover_states()
    n_states = len(index_hash_map)

    S = np.zeros((n_states, 1), dtype = object)

    initial_states = State.get_initial_states()
    for initial_state in initial_states:
        #print(initial_state.to_string())
        initial_state_hash = initial_state.get_hash()
        initial_state_index = hash_index_map[initial_state_hash]
        S[initial_state_index, 0] += 1

    T = np.zeros((n_states, n_states), dtype = object)
    for curr_state_hash, curr_state in hash_state_map.items():
        curr_state_index = hash_index_map[curr_state_hash]
        for next_state in curr_state.get_next_states():
            #print(curr_state.to_string(), "->", next_state.to_string())
            next_state_hash = next_state.get_hash()
            next_state_index = hash_index_map[next_state_hash]
            T[next_state_index, curr_state_index] += 1


    S = np.matmul(pow_mod(T, n-1, MOD), S)
    S = np.mod(S, MOD)

    ans = np.sum(S)

    ans %= MOD

    return ans

if __name__ == "__main__":
    print(solve(N))
