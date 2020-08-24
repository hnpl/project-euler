# Problem: https://projecteuler.net/problem=389

import numpy as np
from scipy.sparse import csr_matrix, lil_matrix

if __name__ == "__main__":
    ans = 0.0

    max_sum = 4*6*8*12*20

    S = lil_matrix((1, max_sum+1), dtype = np.float)
    S[0,1] = S[0,2] = S[0,3] = S[0,4] = 1/4

    prev_S = None

    prev_min_sum = 1
    prev_max_sum = 4
    for n_sides in [6, 8, 12, 20]:
        S, prev_S = prev_S, S

        S = lil_matrix((1, max_sum+1), dtype = np.float)

        dist = lil_matrix((1, max_sum+1), dtype = np.float)
        T = lil_matrix((max_sum+1, max_sum+1), dtype = np.float)

        curr_max_sum = prev_max_sum * n_sides

        for row in range(1, curr_max_sum+1):
            if row + n_sides >= curr_max_sum:
                col_idx = np.arange(curr_max_sum - row) + 1 + row
            else:
                col_idx = np.arange(n_sides) + 1 + row
            T[row, col_idx] = 1/n_sides


        for i in range(1, n_sides+1):
            dist[0,i] = 1/n_sides

        S = S.tocsr()
        dist = dist.tocsr()
        T = T.tocsr()

        S += prev_S[0, prev_min_sum] * dist

        for i in range(prev_min_sum+1, prev_max_sum + 1):
            dist = dist.dot(T)
            S += prev_S[0,i] * dist

        prev_max_sum = curr_max_sum

    S = S.toarray()

    X = np.arange(max_sum+1)
    X_2 = (np.arange(max_sum+1)) ** 2

    ans = np.sum(S*X_2) - (np.sum(S*X))**2

    print(ans)