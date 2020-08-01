# https://projecteuler.net/problem=91

# Fast enough

"""
Note that:
    OP^2 = X_P^2 + Y_P^2
    OQ^2 = X_Q^2 + Y_Q^2
    PQ^2 = (X_P - X_Q)^2 + (Y_P - Y_Q)^2
There are 3 cases:
    1. OP^2 + OQ^2 = PQ^2
        -> X_P * X_Q + Y_P * Y_Q = 0
    2. OP^2 + PQ^2 = OQ^2
        -> X_P^2 + Y_P^2 - X_P * X_Q - Y_P * Y_Q = 0
    3. OQ^2 + PQ^2 = OP^2
        -> X_Q^2 + Y_Q^2 - X_P * X_Q - Y_P * Y_Q = 0
"""

N = 50

ans = 0

for xP in range(N+1):
    for yP in range(N+1):
        if xP == 0 and yP == 0:
            continue
        alpha = xP**2 + yP**2
        for xQ in range(N+1):
            for yQ in range(N+1):
                if xQ == 0 and yQ == 0:
                    continue
                if xP == xQ and yP == yQ:
                    continue
                beta = xQ**2 + yQ**2
                gamma = xP * xQ + yP * yQ
                if gamma*(alpha - gamma)*(beta - gamma) == 0:
                    ans = ans + 1
# For each pair (P = (x1, y1), Q = (x2, y2)), we have that (P = (x2, y2), Q = (x1, y1)) is also counted a solution.
# So we divide the current answer by 2.
print(ans//2)
                