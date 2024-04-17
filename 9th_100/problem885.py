N = 18
MOD = 1123455689

factorial = [1 for i in range(N+1)]
for i in range(1, N+1):
    factorial[i] = factorial[i-1] * i
factorial_inv_mod = [pow(factorial[i], -1, MOD) for i in range(N+1)]

def dfs(depth, arr, curr_digit, curr_num_digits, max_num_digits, count):
    ans = 0
    if curr_digit == 0:
        arr[0] = max_num_digits - curr_num_digits
        count[0] += 1
        n_numbers = factorial[N]
        for digit, n_digits in enumerate(arr):
            n_numbers *= factorial_inv_mod[n_digits]
            n_numbers %= MOD
        # constructing f(x)
        f_str = []
        for digit, n_digits in enumerate(arr):
            for i in range(n_digits):
                f_str.append(str(digit))
        f_str = "".join(f_str)
        f = int(f_str, 10)
        ans = (n_numbers * f) % MOD
        return ans
    ans += dfs(depth+1, arr, 0, curr_num_digits, max_num_digits, count)
    for next_digit in range(curr_digit-1, 0, -1):
        for next_digit_count in range(1, max_num_digits - curr_num_digits + 1):
            arr[next_digit] = next_digit_count
            ans += dfs(depth+1, arr, next_digit, curr_num_digits + next_digit_count, max_num_digits, count)
            ans %= MOD
            arr[next_digit] = 0
    return ans

count = [0]
ans = dfs(0, [0]*10, 10, 0, N, count)
print(count)
print(ans)
