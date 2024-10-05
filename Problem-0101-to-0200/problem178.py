# Problem: https://projecteuler.net/problem=178

"""
    Let DP[a][b][c] be the number of numbers of length <= a, having bit coverage of b and the last digit of c.
    Bit coverage is define as a set S of 10 bits, we S[i] = 1 means the digit i appears in the number, and S[i] = 0 otherwise.

    We have, for each DP[a][b][c], there are two cases:
        Case 1: DP[a][b][c] is built from either DP[a-1][b][c-1] or DP[a-1][b][c+1].
        Case 2: DP[a][b][c] is build from DP[a-1][p][c-1] or DP[a-1][p][c+1], where p is the same bit coverage as b, but it doesn't have bit c set. In other words, digit c is first introduced here.

"""

N = 40

DIGITS = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

N_STATES = 2**(len(DIGITS))

BIT_MASKS = [0] * len(DIGITS)

for i in range(len(DIGITS)):
    BIT_MASKS[i] = 1 << i

if __name__ == "__main__":
    ans = 0
    DP = [[[0 for k in range(len(DIGITS))] 
              for j in range(N_STATES)] 
              for i in range(N+1)]
    for length in range(1, N+1):
        for digit in DIGITS[1:]: # a number doesn't start with a digit 0 
            v = [0] * len(DIGITS)
            v[digit] = 1
            state = get_state(v)
            DP[length][BIT_MASKS[digit]][digit] = 1  # since we also count numbers of length shorter than 'length'
                                                     # we add 1 here to start counting the numbers of length (N - length + 1)
        for digit in DIGITS:
            for prev_state in range(1, N_STATES):
                prev_digits = []
                if digit > 0:
                    prev_digits.append(digit - 1)
                if digit < 9:
                    prev_digits.append(digit + 1)
                if (prev_state & BIT_MASKS[digit]) == 0:
                    DP[length][prev_state | BIT_MASKS[digit]][digit] += sum([DP[length-1][prev_state][prev_digit] for prev_digit in prev_digits])
                else:
                    DP[length][prev_state][digit] += sum([DP[length-1][prev_state][prev_digit] for prev_digit in prev_digits])

    ans = sum([DP[N][0b1111111111][digit] for digit in DIGITS])
    print(ans)

