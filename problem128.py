# Question: https://projecteuler.net/problem=128

from sage.all import *

N = 2000

def PD(n, neighbors):
    count = 0
    for neighbor in neighbors:
        if is_prime(abs(n - neighbor)):
            count = count + 1
    return count

class POSITION:
    A, B, C, D, E, F = 1, 2, 3, 4, 5, 6

class Hints:
    def __init__(self, position, k, layer):
        self.position = position
        self.k = k
        self.layer = layer

class Neighbors:
    def __init__(self):
        self.nA = -1 # TOP
        self.nB = -1 # TOP LEFT
        self.nC = -1 # BOTTOM LEFT
        self.nD = -1 # BOTTOM
        self.nE = -1 # BOTTOM RIGHT
        self.nF = -1 # TOP RIGHT
    def get(self):
        return [self.nA, self.nB, self.nC, self.nD, self.nE, self.nF]

# a(n): generating function of the series 2, 8, 20, ... (POSITION.A of 1)
a = lambda n: 3*n*n - 3*n + 2 # always even

# b(n): generating function of the series 3, 10, 23, ... (POSITION.B of 1)
b = lambda n: 3*n*n - 2*n + 2 # n even -> b(n) is even; n odd -> b(n) is odd

# c(n): generating function of the series 4, 12, 26, ... (POSITION.C of 1)
c = lambda n: 3*n*n - 1*n + 2 # always even

# d(n): generating function of the series 5, 14, 29, ... (POSITION.D of 1)
d = lambda n: 3*n*n       + 2 # n even -> d(n) is even; n odd -> d(n) is odd

# e(n): generating function of the series 6, 16, 32, ... (POSITION.E of 1)
e = lambda n: 3*n*n + 1*n + 2 # always even

# f(n): generating function of the series 7, 18, 35, ... (POSITION.F of 1)
f = lambda n: 3*n*n + 2*n + 2 # n even -> f(n) is even; n odd -> f(n) is odd


def get_neighbors(n, hints):
    neighbors = Neighbors()
    layer = hints.layer
    k = hints.k # n = a(layer) + k, so n is even iff k is even, or n and k have the same parity



    if hints.position == POSITION.A:
        neighbors.nA = a(layer + 1) + k             # diff is even, non-prime (diff = a(layer+1) + k - a(layer) - k)
        neighbors.nB = a(layer + 1) + k + 1
        neighbors.nC = n + 1                        # diff is 1, non-prime
        neighbors.nD = a(layer - 1) + k             # diff is even, non-prime
        if k == 0:
            neighbors.nE = a(layer + 1) - 1
            neighbors.nF = a(layer + 2) - 1
        else:
            neighbors.nE = a(layer - 1) + k - 1 
            neighbors.nF = n - 1                    # k != 0: diff is 1, non-prime
                                                    # so, for every n having POSITION.A and k != 0, 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)


    elif hints.position == POSITION.B: # n = b(layer) + k
        neighbors.nB = b(layer + 1) + k             # either diff of (nB, n) xor (nC, n) is non-prime (one of two consecutive integers is even)
        neighbors.nC = b(layer + 1) + k + 1         # as above
        neighbors.nD = n + 1                        # diff is 1, non-prime
        neighbors.nE = b(layer - 1) + k
        if k == 0:
            neighbors.nA = b(layer + 1) - 1         # k == 0: diff = b(layer + 1) - 1 - b(layer) - k = b(layer + 1) - b(layer) - 1
                                                    # -> diff is even as b(layer+1) - b(layer) is odd (b(n) alters between odd and even)
            neighbors.nF = n - 1                    # diff is 1, non-prime
        else:
            neighbors.nA = n - 1                    # diff is 1, non-prime
            neighbors.nF = b(layer - 1) + k - 1     # k != 0: diff = b(layer) + k - b(layer - 1) - k + 1 = b(layer) - b(layer - 1) + 1
                                                    # -> diff is even as b(layer) - b(layer-1) is odd (b(n) alters between odd and even)

                                                    # so, for every n having POSITION.B and regardless of k = 0 or k != 0, 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)

    elif hints.position == POSITION.C: # n = c(layer) + k
        neighbors.nC = c(layer + 1) + k             # either diff of (nC, n) xor (nD, n) is non-prime (one of two consecutive integers is even)
        neighbors.nD = c(layer + 1) + k + 1         # as above
        neighbors.nE = n + 1                        # diff is 1, non-prime
        neighbors.nF = c(layer - 1) + k             # diff = c(layer) + k - c(layer - 1) - k = c(layer) - c(layer - 1) (even, as c(n) is even)
        if k == 0:
            neighbors.nA = n - 1                    # diff is 1, non-prime
            neighbors.nB = c(layer + 1) - 1
        else:
            neighbors.nA = c(layer - 1) + k - 1
            neighbors.nB = n - 1                    # diff is 1, non-prime

                                                    # so, for every n having POSITION.C and regardless of k = 0 or k != 0, 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)

    elif hints.position == POSITION.D: # n = d(layer) + k
        neighbors.nA = d(layer - 1) + k
        neighbors.nD = d(layer + 1) + k             # either diff of (nD, n) xor (nE, n) is non-prime (one of two consecutive integers is even)
        neighbors.nE = d(layer + 1) + k + 1         # as above
        neighbors.nF = n + 1                        # diff is 1, non-prime
        if k == 0:
            neighbors.nB = n - 1                    # diff is 1, non-prime
            neighbors.nC = d(layer + 1) - 1        
                                                    # -> diff is even, as d(n) alters between odd and even, so d(layer + 1) - d(layer) is odd, so d(layer + 1) - d(layer) - 1 is even
        else:
            neighbors.nB = d(layer - 1) + k - 1     # either diff of (nA, n) xor (nB, n) is non-prime (one of two consecutive integers is even)
            neighbors.nC = n - 1                    # diff is 1, non-prime

                                                    # so, for every n having POSITION.D and regardless of k = 0 or k != 0, 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)

    elif hints.position == POSITION.E: # n = e(layer) + k
        neighbors.nA = n + 1                        # diff is 1, non-prime
        neighbors.nB = e(layer - 1) + k             # diff = e(layer) + k - e(layer - 1) - k = e(layer) - e(layer - 1), so diff is even, as e(n) is even
        neighbors.nE = e(layer + 1) + k             # either diff of (nE, n) xor (nF, n) is non-prime (one of two consecutive integers is even)
        neighbors.nF = e(layer + 1) + k + 1         # as above
        if k == 0:
            neighbors.nC = n - 1                    # diff is 1, non-prime
            neighbors.nD = e(layer + 1) - 1
        else:
            neighbors.nC = e(layer - 1) + k - 1
            neighbors.nD = n - 1                    # diff is 1, non-prime

                                                    # so, for every n having POSITION.E and regardless of k = 0 or k != 0, 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)

    elif hints.position == POSITION.F: # n = f(layer) + k
        neighbors.nA = f(layer + 1) + k + 1         # either diff of (nA, n) xor (nF, n) is non-prime (one of two consecutive integers is even)
        neighbors.nF = f(layer + 1) + k             # as above
        if k == 0:
            neighbors.nD = n - 1                    # diff is 1, non-prime
            neighbors.nE = f(layer + 1) - 1         # diff = f(layer + 1) - 1 - f(layer) - k = f(layer + 1) - f(layer) - 1
                                                    # -> diff is even, as f(n) alters between odd and even, so f(layer + 1) - f(layer) is odd, so f(layer + 1) - f(layer) - 1 is even
        else:
            neighbors.nD = f(layer - 1) + k - 1     # diff = f(layer) + k - f(layer - 1) - k + 1 = f(layer) - f(layer - 1) + 1
                                                    # -> diff is even, as f(n) alters between odd and even, so f(layer) - f(layer - 1) is odd, so f(layer) - f(layer - 1) + 1 is even 
            neighbors.nE = n - 1                    # diff is 1, non-prime
        if k == layer - 1:
            neighbors.nB = a(layer)
            neighbors.nC = a(layer - 1)
        else:
            neighbors.nB = n + 1                    # diff is 1, non-prime
            neighbors.nC = f(layer - 1) + k

                                                    # so, for every n having POSITION.F and k \in [0 ... layer - 2], 
                                                    # we have PD(n) is at most 2 (there are 4 non-primes)

                                                    # In conclusion, we only need to check the case where n = a(layer) and n = f(layer) + layer - 1 = a(layer+1) - 1
    return neighbors

def get_Nth_PD3(N):
    count = 1
    layer = 0
    n = 1
    # calculate layer 1
    # the function labels '1' as '2', here we manually fix it
    layer = layer + 1
    for side in [POSITION.A, POSITION.B, POSITION.C, POSITION.D, POSITION.E, POSITION.F]:            
        for k in range(layer): # layer 1: k \in {0}, layer 2: k \in {0, 1}, ...
            n = n + 1
            neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
            if side == POSITION.A: # 2
                neighbors.nD = 1
            elif side == POSITION.B: # 3
                neighbors.nE = 1
            elif side == POSITION.C: # 4
                neighbors.nF = 1
            elif side == POSITION.D: # 5
                neighbors.nA = 1
            elif side == POSITION.E: # 6
                neighbors.nB = 1
            elif side == POSITION.F: # 7
                neighbors.nC = 1
            if PD(n, neighbors.get()) == 3:
                count = count + 1
    while count < N:
        layer = layer + 1
        """
        for side in [POSITION.A, POSITION.B, POSITION.C, POSITION.D, POSITION.E, POSITION.F]:
            for k in range(layer): # layer 1: k \in {0}, layer 2: k \in {0, 1}, ...
                n = n + 1
                neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
                if PD(n, neighbors.get()) == 3:
                    count = count + 1
                    if count >= N:
                        return n
        """
        
        # diff for k \in {1, 2, ..., layer - 1} are similar, so we only need to check one of them
        # except for k = layer - 1 of POSITION.F
        
        # Case 1: side A -> E
        for side in [POSITION.A, POSITION.B, POSITION.C, POSITION.D, POSITION.E]:
            # Case 1.1: k = 0
            k = 0
            n = n + 1
            neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
            if PD(n, neighbors.get()) == 3:
                count = count + 1
                if count == N:
                    return n
            # Case 1.2: k = 1 -> layer - 1
            k = 1
            n = n + 1
            neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
            if PD(n, neighbors.get()) == 3:
                count = count + layer - 1
                if count >= N:
                    old_count = count - layer + 1
                    return n - 1 + (N - old_count)
            n = (n - 1) + (layer - 1)
        # Case 2: side F
        side = POSITION.F
        # Case 2.1: k = 0
        k = 0
        n = n + 1
        neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
        if PD(n, neighbors.get()) == 3:
            count = count + 1
            if count == N:
                return n
        # Case 2.2: k = 1 -> layer - 2
        k = 1
        n = n + 1
        neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
        if PD(n, neighbors.get()) == 3:
            count = count + layer - 2
            if count >= N:
                old_count = count - layer + 1
                return n - 1 + (N - old_count)
        n = (n - 1) + (layer - 2)
        # Case 2.3: k = layer - 1
        k = layer - 1
        n = n + 1
        neighbors = get_neighbors(n, Hints(position = side, k = k, layer = layer))
        if PD(n, neighbors.get()) == 3:
            count = count + 1
            if count == N:
                return n
    return -1

# even better solution
def get_Nth_PD3_fast(N):
    count = 0
    layer = 0
    while count < N:
        layer = layer + 1
        a_layer = a(layer)
        neighbors = get_neighbors(a_layer, Hints(position = POSITION.A, k = 0, layer = layer))
        if PD(a_layer, neighbors.get()) == 3:
            count = count + 1
            if count == N:
                return a_layer
        f_layer = a(layer+1) - 1
        neighbors = get_neighbors(a_layer, Hints(position = POSITION.F, k = layer - 1, layer = layer))
        if PD(f_layer, neighbors.get()) == 3:
            count = count + 1
            if count == N:
                return f_layer
    return -1



N = 2000
result1 = get_Nth_PD3_fast(N)
result2 = get_Nth_PD3(N)
assert(result1 == result2)
print(result1)
