# Question: https://projecteuler.net/problem=162

N = 16

"""
X -> event that 0 is not in the number
Y -> event that 1 is not in the number
Z -> event that A is not in the number

result = #(~X & ~Y & ~Z)

#(~X & ~Y & ~Z) = all_cases - #(X | Y | Z)
                =   16**N   - #(X | Y | Z)

#(X | Y | Z) = #(X & Y & Z) + #(~X & Y & Z) + #(X & ~Y & ~Z) + #(X & Y & ~Z) + #(~X & ~Y & Z) + #(~X & Y & ~Z) + #(X & ~Y & ~Z)

#(~X & Y & Z) = #(Y & Z) - #(X & Y & Z)
#(X & ~Y & Z) = #(X & Z) - #(X & Y & Z)
#(X & Y & ~Z) = #(X & Y) - #(X & Y & Z)

#(~X & ~Y & Z) = #(Z) - #(~X & Y & Z) - #(X & ~Y & Z) - #(X & Y & Z)
#(~X & Y & ~Z) = #(Y) - #(~X & Y & Z) - #(X & Y & ~Z) - #(X & Y & Z)
#(X & ~Y & ~Z) = #(X) - #(X & ~Y & Z) - #(X & Y & ~Z) - #(X & Y & Z)

So,
#(X | Y | Z) = #(X & Y & Z) - #(X & Y) - #(X & Z) - #(Y & Z) + #(X) + #(Y) + #(Z)
             =     13**N    -   14**N  -  14**N   -  14**N   + 15**N + 15**N + 15**N
So,
#(~X & ~Y & ~Z) = 16**N - 3 * (15**N) + 3 * (14**N) - 13**N

Corner cases: the last digits only contain non-zero numbers, we don't count those cases
"""

n_corner_cases = 2 # 000...001A, 000...00A1
for i in range(3, N):
    # i is the length of no-zero-digits part
    n_corner_cases = n_corner_cases + 15**i - 14**i - 14**i + 13**i
    # 15**i: does not contain 0
    # 14**i: does not contain 0 and 1
    # 14**i: does not contain 0 and A
    # 13**i: does not contain 0 and 1 and A
ans = 16**N - 3 * (15**N) + 3 * (14**N) - 13**N - n_corner_cases
print(hex(ans).upper()[2:])
