N = 16

dont_know = 10**17 - 1
max_n_digits = 17

def get_num(l):
    n = 0
    for d in l:
        n *= 10
        n += d
    return n

def get_digits(n):
    digits = []
    while n > 0:
        digits.append(n % 10)
        n //= 10
    return digits

def num_to_freq(n):
    freq = [0] * 10
    for digit in get_digits(n):
        freq[digit] += 1
    return freq

class Bounds:
    def __init__(self, lowerbound, upperbound):
        self.lowerbound = lowerbound
        self.upperbound = upperbound
    def is_valid(self):
        return self.lowerbound <= self.upperbound
    def intersects(self, other):
        return Bounds(max(self.lowerbound, other.lowerbound), min(self.upperbound, other.upperbound))
    def __str__(self):
        return "[{}, {}]".format(self.lowerbound, self.upperbound)

def dfs(depth, freq, prev_digit, curr_lhs_sum, exp, target_bounds, ans):
    if depth > 1:
        for n in [curr_lhs_sum-1, curr_lhs_sum+1]:
            rhs_freq = num_to_freq(n)
            if rhs_freq == freq:
                ans[0] += n
                print(n)
                return

    for next_digit in range(prev_digit, 10):
        freq[next_digit] += 1

        next_lhs_sum = curr_lhs_sum + next_digit ** exp
        lhs_bounds = Bounds(next_lhs_sum, next_lhs_sum + (max_n_digits - depth - 1) * (9 ** exp))   # lhs: digit power sum
        rhs_bounds = Bounds(10**(depth), dont_know)                                                 # rhs: n
        new_bounds = lhs_bounds.intersects(rhs_bounds)
        new_bounds = new_bounds.intersects(target_bounds)
        if not new_bounds.is_valid():
            freq[next_digit] -= 1
            continue

        dfs(depth+1, freq, next_digit, next_lhs_sum, exp, new_bounds, ans)

        freq[next_digit] -= 1


if __name__ == "__main__":
    ans = [0]
    for n in range(2, N+1):
        print("k =", n)
        dfs(0, [0] * 10, 0, 0, n, Bounds(0, max_n_digits * (9 ** n)), ans)
    print(ans[0])

