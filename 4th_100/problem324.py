# Problem: https://projecteuler.net/problem=324

"""
    - In the problem, we have to fill a 3x3xn tower by a 2x1x1 shape and its rotated shapes.
    - Let a layer be a 3x3x1 shape such that the tower consists of n layers.
    - The strategy is to count how many 3x3 board can be reached from the previous layer.
    - Note that each layer must by completely filled.

    (*) Types of layers
    - We have to fill each layer by 2x1x1, 1x2x1, 1x1x2 shapes.
    - Note that, 2x1x1 and 1x2x1 shapes fit in one layer while the 1x1x2 shape must be in 2 layers, so, if we have a head of 1x1x2 shape in one layer, the next layer must have the tail of the shape.
    - Let bits[9] be an array of bits indicating how a layer's cells is filled 
        where bits[i] = 0 means the row i // 3, column i % 3 of the layer is filled by one of the two tiles of the 2x1x1 or 1x2x1 shapes, or by the tail of the 1x1x2 shape.
              bits[i] = 1 means the row i // 3, column i % 3 of the layer is filled by the head of the 1x1x2 shape.
    - The arrangement of the shape of a layer does not depend on the arrangement of the previous layer, EXCEPT that if the previous layer has heads of 1x1x2 shape, the corresponding tiles in the next layer must be filled by the tail of 1x1x2 shape.
      This means 2 things,
        1. It's appropriate use bits[9] as a hash to differentiate the layers.
        2. From a hash of a layer, we can determine the possibility of the arrangements of the next layer by,
            - Starting from a blank layer.
            - If the previous layer has heads of 1x1x2 shape, filling the corresponding tiles in this layer by the tail of 1x1x2 shape.
            - Then, using DFS to fill the rest of the layer by 2x1x1 and 1x2x1 shapes and the head of 1x1x2 shape.

    (**) Transition matrix
    - From the algorithm above, we start from a completely blank board and use BFS to find the states that can be reached, get the hash of the states, and map each hash to an index in the transition matrix.
    - From the transition matrix, use the idea of Markov process to calculate the number of arrangements of each state for each number of layers.
        i.e., let S_0 be the number of arrangements of each state for 0 layers, in other words, S_0[hash = 0] = 1 and S_0[other hashes] = 0
        then, S_N = S * (T ** N) is the number of arrangements of each state for N layer.
        If N is the final layer, then S_N[hash = 0] contains the result.
    - Now we have that, T is a 252x252 matrix and N = 10*10000.
      T is far too big to calculate (T**N) in an appropriate amount of time.
    - The transition matrix basically encodes a set of linear recurrence relations -> the sequence of S_i[hash = 0] can be encoded by a linear recurrence.

    (***) Determine the linear recurrence generating the result
    - Knowing that the result has a generating linear recurrence, we use Reeds-Sloane Algorithm [https://mathworld.wolfram.com/Reeds-SloaneAlgorithm.html, http://neilsloane.com/doc/Me111.pdf], an extension of Berlekamp-Massey Algorithm [https://mathworld.wolfram.com/Berlekamp-MasseyAlgorithm.html], to find the recurrence.
    - Note that, for odd N, S_N[hash = 0] = 0.
    - So, by applying the algorithm on the first 504 (252*2) non-zero terms of S_i, we have that, we can calculate S_i[hash = 0] from the previous 19 terms.
    - Then we set up a 19x19 matrix M (like what we did in problem 258) to calculate the n-th term using modular exponentiation.
      (there must be a much better way to calculate the n-th term of a linear recurrence)

"""

from sage.all import *
from sage.matrix.berlekamp_massey import berlekamp_massey

from collections import defaultdict

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
        n_empty_tiles = State.__width * State.__height
        while occupy_hash > 0:
            if (occupy_hash & mask(1)) == 1:
                n_empty_tiles = n_empty_tiles - 1
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

    # the internal state_hash is not exposed to the outside of this class
    # so here, we do the reverse of get_state_hash(), which means,
    # we'll put the fences back
    def init_full_state_from_state_hash(self, state_hash):
        self.state_hash = 0
        object_state_hash_msb_index = 0
        for row in range(State.__height):
            self.state_hash = self.state_hash | ((state_hash & mask(State.__width-1)) << object_state_hash_msb_index)
            object_state_hash_msb_index += State.__width - 1
            # placing the fence
            # in state_hash, a bit representing fence is labeled '0'
            self.state_hash = self.state_hash | (0 << object_state_hash_msb_index)
            object_state_hash_msb_index += 1
            state_hash = state_hash >> (State.__width - 1)
        self.occupy_hash = mask(State.__width * State.__height)
        return self

    def init_from_unique_hash(self, unique_hash):
        self.state_hash = unique_hash & mask(State.__width * State.__height)
        self.occupy_hash = unique_hash >> (State.__width * State.__height)
        self.n_empty_tiles = self.__count_n_empty_tiles()
        return self

    def add_tile(self, row, col, label): # add a single tile, i.e. tail of shape3
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

    def get_next_layer_initial_state(self):
        next_layer_initial_state = State().__init_from_hashes(state_hash = 0,
                                                              occupy_hash = self.state_hash | State.__get_initial_occupy_hash())
        return next_layer_initial_state

    def describe_state(self):
        print("State", bin(self.state_hash), " Occupying ", bin(self.occupy_hash), "Empty ", self.n_empty_tiles)

def merge_freq_defaultdicts(freq1, freq2):
    assert(isinstance(freq1, defaultdict))
    assert(isinstance(freq2, defaultdict))
    freq = freq1.copy()
    for key, val in freq2.items():
        freq[key] += val
    return freq

cache = {}

def dfs(state, possible_shapes):
    if state.is_full():
        freq = defaultdict(lambda: 0)
        freq[state.get_state_hash()] = 1
        #state.describe_state()
        return freq

    if state.get_hash() in cache: # do not use state_hash, which does not encode positions of occupied tiles
        return cache[state.get_hash()].copy()
    
    freq = defaultdict(lambda: 0)
    for next_state in state.generate_next_states(possible_shapes):
        next_state_freq = dfs(next_state, possible_shapes)
        freq = merge_freq_defaultdicts(freq, next_state_freq)

    if not state.get_hash() in cache:
        cache[state.get_hash()] = freq

    return freq
    
def get_state_permutations(inital_state, possible_shapes):
    full_states_freq = dfs(inital_state, possible_shapes)
    return full_states_freq

if __name__ == "__main__":
    ans = 0

    shapes = Shape.get_possible_shapes()

    inital_state = State()

    state_space_next_index = 0
    state_space = {}
    state_hash_index_map = {}

    count = 0

    first_full_state_hash = State().init_full_state_from_state_hash(0).get_state_hash()

    to_explore = [first_full_state_hash]

    # Try all ways to place shape 3, and then fill the rest with shape 1 and shape 2
    # If the previous layer has shape 3, the next layer will have the corresponding tiles occupied (tail of shape 3)
    #for state_hash in range(2**9):
    while len(to_explore) > 0:
        full_state_hash = to_explore[0]
        del to_explore[0]
        
        state = State().init_full_state_from_state_hash(full_state_hash)
        #state.describe_state()

        next_layer_initial_state = state.get_next_layer_initial_state()
        #next_layer_initial_state.describe_state()
        full_states_freq = get_state_permutations(next_layer_initial_state, shapes)
        state_space[full_state_hash] = full_states_freq.copy()
        
        #print(full_states_freq)

        if not full_state_hash in state_hash_index_map:
            state_hash_index_map[full_state_hash] = state_space_next_index
            state_space_next_index += 1

        for next_state_hash in full_states_freq.keys():
            if not next_state_hash in state_hash_index_map:
                state_hash_index_map[next_state_hash] = state_space_next_index
                state_space_next_index += 1
                to_explore.append(next_state_hash)
 

    n_states = len(state_space)

    R = IntegerModRing(MOD)

    ## building the transition matrix, certainly not a sparse matrix
    T = matrix(R, n_states, n_states, lambda i, j: 0)
    for state_hash, next_states_freq in state_space.items():
        for next_state_hash, next_state_freq in next_states_freq.items():
            T[state_hash_index_map[state_hash], state_hash_index_map[next_state_hash]] = next_state_freq

    S = matrix(R, 1, n_states, lambda i, j: 0)
    S[0, state_hash_index_map[0]] = 1


    #results = []
    #for i in range(n_states*4):
    #    S = S * T
    #    if i % 2 == 1: # even i emits zero
    #        results.append(S[0, 0])

    results = []
    T_squared = T**2
    for i in range(n_states*2):
        S = S * T_squared
        results.append(S[0,0])
    

    ## Use Berlekamp-Massey algorithm to find the minimal polynomial representing the recurrence relation
    polynomial = berlekamp_massey(results)

    ## characteristic polynomial = x^19 + 99999332*x^18 + 73471*x^17 + 96778818*x^16 + 72583272*x^15 + 74091806*x^14 + 71102733*x^13 + 76943940*x^12 + 71520743*x^11 + 95475903*x^10 + 4524104*x^9 + 28479264*x^8 + 23056067*x^7 + 28897274*x^6 + 25908201*x^5 + 27416735*x^4 + 3221189*x^3 + 99926536*x^2 + 675*x + 100000006
    ## this means, the recurrence relation that generates the result is:
    ##   A_(k+19) = -99999332*A_(k+18) - 73471*A_(k+17) - 96778818*A_(k+16) - 72583272*A_(k+15) - 74091806*A_(k+14) - 71102733*A_(k+13) - 76943940*A_(k+12) - 71520743*A_(k+11) - 95475903*A_(k+10) - 4524104**A_(k+9) - 28479264**A_(k+8) - 23056067**A_(k+7) - 28897274**A_(k+6) - 25908201**A_(k+5) - 27416735**A_(k+4) - 3221189**A_(k+3) - 99926536**A_(k+2) - 675*A_(k+1) - 100000006 * A_k

    n_prev_terms = len(polynomial.coefficients()) - 1 # we don't count the highest degree's term (i.e. the x^19 term)

    ## generate the matrix that calculates the above recurrence relation, similar to lagged Fibonacci sequence (problem 258)
    T = matrix(R, n_prev_terms, n_prev_terms, lambda i, j: 1 if i == j - 1 else 0)
    for idx, coeff in enumerate(reversed(polynomial.coefficients()[:-1])):
        T[idx, 0] = -coeff

    S = reversed(results[:n_prev_terms])
    S = matrix(R, S)

    S = S * (T ** (N // 2 - n_prev_terms))
    ans = lift(S[0,0])

    print(ans)
