# Question: https://projecteuler.net/problem=175

# From: https://oeis.org/A002487, the first few elements form the Stern-Brocot sequence.

# https://en.wikipedia.org/wiki/Stern%E2%80%93Brocot_tree

import math

class Fraction: # p/q
    def __init__(self, p, q, need_to_be_reduced=False):
        self.p = p
        self.q = q
        if need_to_be_reduced:
            self.reduce()
    def __eq__(self, rhs):
        return self.p == rhs.p and self.q == rhs.q
    def __gt__(self, rhs):
        return self.p / self.q >= rhs.p / rhs.q
    def __str__(self):
        return "{}/{}".format(self.p, self.q)
    def reduce(self):
        cd = math.gcd(self.p, self.q)
        self.p //= cd
        self.q //= cd


target = Fraction(123456789, 987654321, True)

def find_mid(lhs, rhs):
    return Fraction(lhs.p + rhs.p, lhs.q + rhs.q)

if __name__ == "__main__":
    left_node = Fraction(0, 1)
    right_node = Fraction(1, 0)
    ans = []
    curr_bit = 0
    curr_bit_count = 0
    curr_node = find_mid(left_node, right_node)
    while not target == curr_node:
        curr_node = find_mid(left_node, right_node)
        next_bit = 0
        if curr_node > target:
            next_bit = 1
            right_node = curr_node
        else:
            next_bit = 0
            left_node = curr_node
        if next_bit != curr_bit:
            if curr_bit_count > 0:
                ans.append(curr_bit_count)
            curr_bit_count = 0
            curr_bit = next_bit
        curr_bit_count += 1
    if curr_bit_count > 0:
        ans.append(curr_bit_count)
    print(list(reversed(ans)))
