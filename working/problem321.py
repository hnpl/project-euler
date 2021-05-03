import sys
from math import log2, sqrt
from decimal import Decimal, getcontext

getcontext().prec = 20

class Array:
    def __init__(self, n):
        self.arr = [0] * (2*n+1)
        for i in range(n):
            self.arr[i] = 1
        for i in range(n+1, 2*n+1):
            self.arr[i] = 2
        self.zero_pos = n
        self.n = n
        self.length = 2*n + 1
        self.hash_width = 2

    def generate_next_states(self, seen_hashes):
        next_states = []
        for dpos in [-2, -1, 1, 2]:
            next_zero_pos = self.zero_pos + dpos
            if next_zero_pos < 0 or next_zero_pos >= self.length:
                continue
            next_state = Array(self.n)
            next_state.arr = self.arr[:]
            t = next_state.arr[self.zero_pos]
            next_state.arr[self.zero_pos] = next_state.arr[next_zero_pos]
            next_state.arr[next_zero_pos] = t
            next_state.zero_pos = next_zero_pos
            next_state_hash = next_state.get_hash()
            if next_state_hash in seen_hashes:
                continue
            seen_hashes.add(next_state_hash)
            next_states.append((next_state, next_state_hash))
        return next_states
        
    def get_hash(self):
        h = 0
        for e in self.arr:
            h <<= self.hash_width
            h += e
        return h

    def get_target_states(n):
        target_states = []
        
        target_state_1 = Array(n)
        for i in range(n):
            target_state_1.arr[i] = 2
        for i in range(n+1, 2*n+1):
            target_state_1.arr[i] = 1
        target_states.append(target_state_1)

        target_state_2 = Array(n)
        target_state_2.arr[0] = 0
        for i in range(1, n+1):
            target_state_2.arr[i] = 2
        for i in range(n+1, 2*n+1):
            target_state_2.arr[i] = 1
#        target_states.append(target_state_2)

        return target_states
                
    def __str__(self):
        return str(self.arr)

    def decode(h, n):
        s = []
        for i in range(2*n+1):
            s.append(h & 0x3)
            h >>= 2
        return list(reversed(s))

def eq1(n):
    sqrt2 = Decimal(2).sqrt()
    return Decimal(-1) + Decimal(1)/8 * (((3-2*sqrt2)**n) * (4 + sqrt2) - (-4 + sqrt2) * (3+2*sqrt2)**n)

def eq2(n):
    sqrt2 = Decimal(2).sqrt()
    return Decimal(-1) + Decimal(1)/8 * (((3-2*sqrt2)**n) * (4 - sqrt2) + (4 + sqrt2) * (3+2*sqrt2)**n)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        n = int(sys.argv[1])
        target_state_hashes = {state.get_hash() for state in Array.get_target_states(n)}


        prev = []
        curr = [Array(n)]
        seen = {curr[0].get_hash()}
        p = {}
        iteration = 0
        f_state = 0
        found = False
        while curr:
            iteration += 1
            curr, prev = [], curr
            for prev_state in prev:
                for next_state, next_state_hash in prev_state.generate_next_states(seen):
                    p[next_state_hash] = prev_state.get_hash()
                    if next_state_hash in target_state_hashes:
                        f_state = next_state_hash
                        found = True
                        break
                    curr.append(next_state)
                if found: break
            if found: break
        print(iteration)

    else:
        l = []
        for i in range(1,40):
            l.append(round(eq1(i)))
            l.append(round(eq2(i)))
        l = sorted(l)
        print(sum(l[:40]))
