# Question: https://projecteuler.net/problem=315

from sage.all import *

BLANK_STATE = 0xFFFFFFFFFFFFFFFFFFFFFFFF

class SEGMENTS:
    v1, v2, v3, h1, h2, h3, h4 = 1, 2, 3, 4, 5, 6, 7

class Digit:
    def __init__(self, n):
        self.n = n
        self.segments = self.get_segments(n)
    def get_segments(self, n):
        if n == 0:
            return {SEGMENTS.v1, SEGMENTS.v3, SEGMENTS.h1, SEGMENTS.h2, SEGMENTS.h3, SEGMENTS.h4}
        elif n == 1:
            return {SEGMENTS.h2, SEGMENTS.h4}
        elif n == 2:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h2, SEGMENTS.h3}
        elif n == 3:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h2, SEGMENTS.h4}
        elif n == 4:
            return {SEGMENTS.v2, SEGMENTS.h1, SEGMENTS.h2, SEGMENTS.h4}
        elif n == 5:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h1, SEGMENTS.h4}
        elif n == 6:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h1, SEGMENTS.h3, SEGMENTS.h4}
        elif n == 7:
            return {SEGMENTS.v1, SEGMENTS.h1, SEGMENTS.h2, SEGMENTS.h4}
        elif n == 8:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h1, SEGMENTS.h2, SEGMENTS.h3, SEGMENTS.h4}
        elif n == 9:
            return {SEGMENTS.v1, SEGMENTS.v2, SEGMENTS.v3, SEGMENTS.h1, SEGMENTS.h2, SEGMENTS.h4}
        elif n == 10:
            return set()
        return None

class Diff1:
    def __init__(self):
        self.segments = [Digit(i).segments for i in range(10)]
    def diff(self, num1, num2):
        if num1 == BLANK_STATE:
            num1 = []
        else:
            num1 = list(map(int, str(num1)))
        if num2 == BLANK_STATE:
            num2 = []
        else:
            num2 = list(map(int, str(num2)))
        df = 0
        for i in num1:
            df = df + len(self.segments[i])
        for i in num2:
            df = df + len(self.segments[i])
        return df

class Diff2:
    def __init__(self):
        self.segments = [Digit(i).segments for i in range(11)]
        self.cache = {}
        for i in range(11):
            for j in range(11):
                self.cache[(i, j)] = self.digit_diff(i, j)
    def digit_diff(self, d1, d2):
        # erase d1, draw d2
        n_erase = len(self.segments[d1] - self.segments[d2])
        n_draw = len(self.segments[d2] - self.segments[d1])
        return n_erase + n_draw
    def diff(self, num1, num2): # we have num1 and num2 having the same length
        if num1 == BLANK_STATE:
            num1 = []
        else:
            num1 = list(map(int, str(num1)))
        if num2 == BLANK_STATE:
            num2 = []
        else:
            num2 = list(map(int, str(num2)))
        while len(num1) > len(num2):
            num2 = [10] + num2
        while len(num2) > len(num1):
            num1 = [10] + num1
        df = 0
        for i, j in zip(num1, num2):
            df = df + self.cache[(i, j)]
        return df

primes = prime_range(10**7, 2*(10**7))

diff1 = Diff1()
diff2 = Diff2()
diff = 0
n1 = 0
n2 = 0

for prime in primes:
    prev_state = BLANK_STATE
    state = prime
    while prev_state >= 10:
        diff = diff + diff1.diff(prev_state, state) - diff2.diff(prev_state, state)
        prev_state, state = state, sum(list(map(int, str(state))))
    prev_state, state = state, BLANK_STATE
    diff = diff + diff1.diff(prev_state, state) - diff2.diff(prev_state, state)
    
print(diff)