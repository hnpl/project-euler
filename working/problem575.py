from sage.all import *
import numpy as np

N = 8

if __name__ == "__main__":
    BASE_TYPE = QQ
    ans = BASE_TYPE(0)

    square_numbers = [i*i for i in range(1, N+1)]

    T = [None, None]

    T[0] = matrix(BASE_TYPE, N*N, N*N, lambda x, y: 0)
    T[1] = matrix(BASE_TYPE, N*N, N*N, lambda x, y: 0)

    for x in range(N):
        for y in range(N):
            neighbors = [(x, y)]
            for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
                nx = x + dx
                ny = y + dy
                if nx < 0 or nx >= N or ny < 0 or ny >= N:
                    continue
                neighbors.append((nx, ny))
                    
            # case 1
            n_neighbors = len(neighbors)
            for nx, ny in neighbors:
                T[0][nx*N+ny, x*N+y] += BASE_TYPE(1/n_neighbors)
            # case 2
            for nx, ny in neighbors:
                if nx == x and ny == y:
                    T[1][nx*N+ny, x*N+y] += BASE_TYPE(1/2)
                else:
                    T[1][nx*N+ny, x*N+y] += BASE_TYPE(1/2) * BASE_TYPE(1/(n_neighbors - 1))

    # solve A * s = I where A = T - I
    A = [None, None]
    for case in range(2):
        I = matrix.identity(N*N)
        A[case] = T[case] - I
        for i in range(N*N):
            A[case][N*N-1, i] = 1
        rhs = matrix(BASE_TYPE, N*N, 1, lambda i, j: 0)
        rhs[N*N-1, 0] = 1
        S = A[case].solve_right(rhs)
    
        for square_number in square_numbers:
            ans += BASE_TYPE(1/2) * S[square_number-1, 0]
        for x in range(N):
            for y in range(N):
                print(S[x*N+y, 0], end=' ')
            print()
        print()
        print()

    print(ans, float(ans))