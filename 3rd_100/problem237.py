# Question: https://projecteuler.net/problem=237

"""
    . Use BFS to count the number of possible tours for small n.
      So, we know the values of T(n) for small n.
    . Use the Berkelamp-Massey algorithm to find the linear recursion of T.
    . Knowing the linear recursion, we can determine the n-th term of T(n) via modular matrix exponentiation (like what we did for problem 324).
"""

from sage.all import *
from sage.matrix.berlekamp_massey import berlekamp_massey
from collections import deque

N = 4
M = 10**12
MOD = 10**8

def mask(n):
    return 2**n-1

MASKS = [mask(i) for i in range(10)]

class State:
    N_POSITION_BITS = 5 # number of bits to represent the position of the cursor
    UNOCCUPIED = 0
    OCCUPIED = 1
    def __init__(self):
        pass
    def init_with_hash(self, width, hash):
        self.width = width # the width of the board
        self._hash = _hash
        return self
    def init_with_board(self, width, board, cursor_x, cursor_y):
        self.width = width
        self._hash = State.encode(width, board, cursor_x, cursor_y)
        return self
    def get_hash(self):
        return self._hash
    def get_initial_state(width):
        board = [[State.UNOCCUPIED for j in range(width)] for i in range(N)]
        cursor_x = 0
        cursor_y = 0
        board[cursor_x][cursor_y] = State.OCCUPIED
        return State().init_with_board(width, board, cursor_x, cursor_y)
    def get_target_state(width):
        board = [[State.OCCUPIED for j in range(width)] for i in range(N)]
        cursor_x = N-1
        cursor_y = 0
        return State().init_with_board(width, board, cursor_x, cursor_y)
    def encode(width, board, cursor_x, cursor_y):
        _hash = 0
        msb_index = 0

        _hash = _hash | (cursor_x << msb_index)
        msb_index += State.N_POSITION_BITS
        _hash = _hash | (cursor_y << msb_index)
        msb_index += State.N_POSITION_BITS

        for i in range(N):
            for j in range(width):
                _hash = _hash | (board[i][j] << msb_index)
                msb_index += 1

        return _hash
    def decode(width, _hash):
        board = [[0 for j in range(width)] for i in range(N)]
        cursor_x = 0
        cursor_y = 0

        h = _hash
        cursor_x = h & MASKS[State.N_POSITION_BITS]
        h >>= State.N_POSITION_BITS
        cursor_y = h & MASKS[State.N_POSITION_BITS]
        h >>= State.N_POSITION_BITS

        for i in range(N):
            for j in range(width):
                board[i][j] = h & MASKS[1]
                h >>= 1

        return board, cursor_x, cursor_y

    def get_next_states(self):
        next_states = []

        board, cursor_x, cursor_y = State.decode(self.width, self._hash)

        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            next_cursor_x = cursor_x + dx
            next_cursor_y = cursor_y + dy
            # boundary check
            if ((next_cursor_x < 0) or (next_cursor_x >= N) or (next_cursor_y < 0) or (next_cursor_y >= self.width)):
                continue
            # check if we can move the cursor to the new position
            if board[next_cursor_x][next_cursor_y] == State.OCCUPIED:
                continue
            board[next_cursor_x][next_cursor_y] = State.OCCUPIED
            next_state = State().init_with_board(self.width, board, next_cursor_x, next_cursor_y)
            next_states.append(next_state)
            board[next_cursor_x][next_cursor_y] = State.UNOCCUPIED
        return next_states

    def print(self):
        board, cursor_x, cursor_y = State.decode(self.width, self._hash)
        for i in range(N):
            for j in range(self.width):
                if board[i][j] == State.OCCUPIED:
                    if cursor_x == i and cursor_y == j:
                        print("*", end='')
                    else:
                        print("x", end='')
                else:
                    print(".", end='')
            print()
        print()

def bfs(width):
    queue = deque()
    initial_state = State.get_initial_state(width)
    target_state = State.get_target_state(width)
    target_state_hash = target_state.get_hash()
    count = 0
    queue.append(initial_state)
    while queue:
        curr_state = queue.popleft()
        for next_state in curr_state.get_next_states():
            if next_state.get_hash() == target_state_hash:
                count += 1
                continue
            queue.append(next_state)
    return count

if __name__ == "__main__":
    P = []
    for width in range(1, 9):
        P.append(bfs(width))
    R = IntegerModRing(10**8)
    
    linear_relation = berlekamp_massey(P)
    coeffs = linear_relation.coefficients()

    n_prev_terms = len(coeffs)-1
    
    S = matrix(R, n_prev_terms, 1, lambda i, j: P[i])

    T = matrix(R, n_prev_terms, n_prev_terms, lambda i, j: 0)
    for i in range(n_prev_terms-1):
        T[(i, i+1)] = 1
    T[n_prev_terms-1] = [-i for i in coeffs[:-1]]
    print(T)

    S_target = (T**(M-n_prev_terms)) * S
    ans = S_target[n_prev_terms-1, 0]

    print(ans)