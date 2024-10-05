# Question: https://projecteuler.net/problem=290

"""
    * Example,
        . Let the current number be "abcde" where each of {a,b,c,d,e} being the digit.
        . Let SD be the digit sum function, then SD("abcde") = a+b+c+d+e.
        . Consider the number "7abcde".
        . We have that,
            SD(137 * "7abcde") = SD(137 * 7 * 100000 + 137 * "abcde")
          Note that, the last 5 digits of 137 * 7 * 100000 are 00000.
          It follows that SD(137 * 7 * 100000 + 137 * "abcde")
                        = SD(137 * 7 + 137 * "abcde" // 100000) + SD(137 * "abcde" % 100000)
        . So, we can generalize as follows,
            . Suppose we have a number n of k-digit.
            . Suppose that SD(the last k digits of 137*n) = beta.
            . Suppose that carry_n = (137*n % 10^k).
            . Consider the number m = n + q * (10^k) (i.e. adding the digit q to the left of the number n).
            . We have that SD(the last k+1 digits of 137 * m) = (137 * q + carry_n % 10)
                       and carry_m = (137 * q + carry_n) // 10
    * For this problem, we will count possible combinations of (carry, diff) for increasing lengths, where carry is described as above, while diff = SD(n) - SD(last k digits of 137*n) for the k-digit number n.
"""

N = 18

digit_sum = lambda n: n if n < 10 else digit_sum(n//10) + (n%10)

if __name__ == "__main__":
    ans = 0

    curr_states = {}
    prev_states = {}

    for n in range(10):
        n137 = 137*n
        diff = n - n137 % 10
        carry = n137 // 10
        state = (diff, carry)
        curr_states[state] = 1

    for length in range(2, N+1):
        prev_states, curr_states = curr_states, {}
        for prev_state, prev_state_count in prev_states.items():
            prev_diff, prev_carry = prev_state
            for next_digit in range(10):
                carry = prev_carry + 137 * next_digit
                next_carry = carry // 10
                next_diff = prev_diff + next_digit - carry % 10
                next_state = (next_diff, next_carry)
                if not next_state in curr_states:
                    curr_states[next_state] = 0
                curr_states[next_state] += prev_state_count

    for state, state_count in curr_states.items():
        diff, carry = state
        diff -= digit_sum(carry)
        if diff == 0:
            ans += state_count

    print(ans)
