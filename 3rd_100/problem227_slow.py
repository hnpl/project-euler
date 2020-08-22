# Problem: https://projecteuler.net/problem=227

# Use Markov process
# T[pos1][pos2] is the probability of transitioning from position 1 to position 2

import numpy as np

N = 100

def position_to_index(x, y):
    return x * N + y

def index_to_position(idx):
    return idx // N, idx % N

def new_position(current_position, roll):
    if roll == 1:
        current_position = (current_position - 1) % N
    elif roll == 6:
        current_position = (current_position + 1) % N
    return current_position

if __name__ == "__main__":
    ans = 0.0

    T = np.zeros((N*N, N*N), dtype = np.double)
    S = np.zeros((N*N), dtype = np.double)

    for i in range(N):
        S[position_to_index(i, (i + 50)%N)] = 1/N

    for position in range(N*N):
        x, y = index_to_position(position)
        if x == y:
            T[position][position] = 1.0
        else:
            new_x_1 = new_position(x, 1)
            new_y_1 = new_position(y, 1)
            new_x_6 = new_position(x, 6)
            new_y_6 = new_position(y, 6)

            # player 1 rolls 1, player 2 rolls 1
            T[position][position_to_index(new_x_1, new_y_1)] += 1/36
            # player 1 rolls 1, player 2 rolls [2..5]
            T[position][position_to_index(new_x_1, y)] += 4/36
            # player 1 rolls 1, player 2 rolls 6
            T[position][position_to_index(new_x_1, new_y_6)] += 1/36

            # player 1 rolls [2..5], player 2 rolls 1
            T[position][position_to_index(x, new_y_1)] += 4/36

            # player 1 rolls [2..5], player 2 rolls 6
            T[position][position_to_index(x, new_y_6)] += 4/36

            # player 1 rolls 6, player 2 rolls 1
            T[position][position_to_index(new_x_6, new_y_1)] += 1/36
            # player 1 rolls 6, player 2 rolls [2..5]
            T[position][position_to_index(new_x_6, y)] += 4/36
            # player 1 rolls 6, player 2 rolls 6
            T[position][position_to_index(new_x_6, new_y_6)] += 1/36

            T[position][position] += 16/36

    prev_probability = 0.0
    curr_probability = 0.0

    for n_turns in range(1, 10000+1):
        prev_probability = curr_probability

        S = S.dot(T)

        curr_probability = 0.0
        for i in range(N):
            curr_probability += S[position_to_index(i, i)]

        ans = ans + n_turns * (curr_probability - prev_probability)

        print(n_turns, ans)

    print(ans)

