from decimal import Decimal, getcontext
from math import floor

M = 24

getcontext().prec = M + 2

def eval(r, precision):
    n_chars = 0
    s = ""
    w = r
    for i in range(M):
        char = str(int(floor(w)))
        s += char
        if i == 0:
            s += "."
        w = floor(w) * (w - floor(w) + 1)
        n_chars += len(char)
        if n_chars >= M:
            s = s[:precision + 2]
            break
    t = Decimal(s)
    return t

# grid search

L = Decimal(2)
step = Decimal("0.1")
n_steps = 10
precision = 1
prev_candidates = []
curr_candidates = [Decimal(2)]
while precision <= M:
    curr_candidates, prev_candidates = [], curr_candidates
    for candidate in prev_candidates:
        delta = 0
        for i in range(n_steps - 1):
            delta += step
            next_candidate = candidate + delta
            if next_candidate == eval(next_candidate, precision):
                curr_candidates.append(next_candidate)
    if not curr_candidates:
        curr_candidates = prev_candidates
        n_steps *= 10
    step /= 10
    precision += 1
    print(curr_candidates)
