# Question: https://projecteuler.net/problem=2

# Fibonacci sequence grows exponentially, so a simple loop should do it, floating point operations are unneccesary as well
# Assume that the instructions are executed in an out-of-order pipeline

# Pattern:     0,    1,    1,    2,    3,    5,    8, ...
#           even   odd   odd  even   odd   odd  even
# the 3k^th elements are even

N = int(4e6)

sumF = 0

f3k, f3k1, f3k2 = 0, 1, 1

while f3k <= N:
    # f3k = f3k1 + f3k2
    # f3k1 = f3k2 + f3k (false/unnecessary dependency w.r.t f3k)
    # f3k2 = f3k + f3k1 (false/unnecessary dependency w.r.t f3k1)
    # sumF = sumF + f3k (false/unneccesary dependency w.r.t f3k)

    # sumF is always one-iteration-late
    f3k, f3k1, f3k2, sumF = f3k1 + f3k2, f3k1 + 2 * f3k2, 2 * f3k1 + 3 * f3k2, sumF + f3k

print(sumF)

# -----------------------------------------------------------------------------
# Even better solution: observe that F_{3k} = 4 * F_{3k-3} + F_{3k-6}
# So, F_{3k} = 4 * F_{3k-3} + F_{3k-6}
#     F_{3k+3} = 4 * F_{3k} + F_{3k-3} = 17 * F_{3k-3} + 4 * F_{3k-6}
#     F_{3k+6} = 4 * F_{3k+3} + F_{3k} = 72 * F_{3k-3} + 17 * F_{3k-6}

sumF2 = 0

f3k, f3k3, f3k6 = 0, 2, 8

while f3k6 <= N:
    # sumF is always one-iteration-late
    f3k, f3k3, f3k6, sumF2 = 4 * f3k6 + f3k3, 17 * f3k6 + 4 * f3k3, 72 * f3k6 + 17 * f3k3, sumF2 + f3k + f3k3 + f3k6

if f3k <= N: sumF2 = sumF2 + f3k
if f3k3 <= N: sumF2 = sumF2 + f3k3
if f3k6 <= N: sumF2 = sumF2 + f3k6


print(sumF2)

assert(sumF == sumF2)