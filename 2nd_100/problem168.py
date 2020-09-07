# Problem: https://projecteuler.net/problem=168

"""
    . From the number of digits, the first number of the rotated number, and the multiplier k, we can reconstruct the number that satisfies the problem statement.
        . Suppose the original number is (digit_1, digit_2, ..., digit_n)
              then the rotated number is (digit_n, digit_1, ..., digit_(n-1))
          Suppose the multiplier is k.
          We have that (digit_n, digit_1, ..., digit_(n-1)) = k * (digit_1, digit_2, ..., digit_n)
          So,
                  carry = 0
            digit_(n-1) = (carry + (k * digit_n)) % 10
                  carry = (carry + (k * digit_n)) // 10
            digit_(n-2) = (carry + (k * digit_(n-1))) % 10
                  carry = (carry + (k * digit_(n-1))) // 10
            and so on
"""

N = 100
MOD = 10**5

def generate_number(rotated_first_digit, k , n_digits):
    rotated_ans = ["0"] * n_digits

    carry = 0
    prev_digit = rotated_first_digit
    for digit_idx_rotated in range(n_digits-1, -1, -1):
        digit = carry + k * prev_digit
        carry = digit // 10
        digit %= 10
        prev_digit = digit
        rotated_ans[digit_idx_rotated] = str(digit)

    if carry > 0 or rotated_ans[1] == "0": # rotated_ans[1] is the first digit of ans, so it can't be 0
        return -1

    if rotated_first_digit == int(rotated_ans[0]):
        rotated_ans.append(rotated_ans[0])
        del rotated_ans[0]
        return int("".join(rotated_ans[-5:]))

    return -1

if __name__ == "__main__":
    ans = 0
    for n_digits in range(2, N+1):
        for rotated_first_digit in range(1, 10):
            for k in range(1, 10):
                l = generate_number(rotated_first_digit, k, n_digits)
                if l >= 0:
                    ans += l
                    ans %= MOD
    print(ans)
