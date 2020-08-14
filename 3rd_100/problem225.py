# Problem: https://projecteuler.net/problem=225

"""
    - Consider an odd number K.
    - Since T_n = T_{n-1} + T_{n-2} + T_{n-3}, the Tribonacci sequence mod K is cyclic (when we encounter (T_{n-1}, T_{n-2}, T{n-3}) again).
        - There are K^3 possibilities of (T_{n-1}%K, T_{n-2}%K, T_{n-3}%K); so we the cycle length is at most K^3.
    - So, iterate through the sequence of the first K^3 values of T_{i} and check whether (T_{i}, T_{i-1}, T{i-2}) has been seen before.
    - If the sequence contains 0, it means one of the Tribonacci sequence is divisible by K.
"""

N = 124
M = 10000

class NQueue: # only hold N integers at a time, discard the oldest item when there are more than N integers

    def __init__(self, n):
        self.q = [0]*n
        self.curr_idx = 0
        self.n = n
        self._hash = 0

    def next_idx(self, i):
        return (i+1) % self.n

    def add(self, v):
        self.q[self.curr_idx] = v
        
        self.curr_idx = self.next_idx(self.curr_idx)

        self._hash = 0
        i = self.next_idx(self.curr_idx)
        stop_i = self.curr_idx
        while not i == stop_i:
            self._hash = (self._hash << 32) + self.q[i]
            i = self.next_idx(i)
        self._hash = (self._hash << 32) + self.q[i]

    def sum(self):
        return sum(self.q)

    def hash(self):
        return self._hash

def check(n):
    mod_seq = NQueue(3)
    mod_seq.add(1)
    mod_seq.add(1)
    mod_seq.add(1)
    seen = { mod_seq.hash() }
    for i in range(n**3):
        next_sum = mod_seq.sum() % n
        if next_sum == 0:
            return False
        mod_seq.add(next_sum)
        h = mod_seq.hash()
        if h in seen:
            return True
        seen.add(h)
    return False

if __name__ == "__main__":

    ans = 0

    count = 0
    curr_odd = 1

    while count < N:
        curr_odd = curr_odd + 2
        if check(curr_odd):
            count = count + 1
            ans = curr_odd

    print(ans)


