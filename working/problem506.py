# https://projecteuler.net/problem=506

N = 10**14
MOD = 123454321

digit_sum = lambda seq: sum(map(int, seq))

prefix_seqs = {
    0: "123423332432122",
    1: "    32423321233".replace(' ', '0'),
    2: "     1 14212344".replace(' ', '0'),
    3: "       2 123433".replace(' ', '0'),
    4: "          34322".replace(' ', '0'),
    5: "              1".replace(' ', '0')
}

prefix_seq_sums = {
    0: digit_sum(prefix_seqs[0]) * 1,
    1: digit_sum(prefix_seqs[1]) * 10,
    2: digit_sum(prefix_seqs[2]) * 100,
    3: digit_sum(prefix_seqs[3]) * 1000,
    4: digit_sum(prefix_seqs[4]) * 10000,
    5: digit_sum(prefix_seqs[5]) * 100000
}

periodic_seqs = {
    0: "123423332432122", # 37
    1: "212332423321233", # 36
    2: "321241314212344", # 37
    3: "432132223123433", # 38
    4: "343223132234322", # 39
    5: "234314241343211"  # 38
}

periodic_seq_sums = {
    0: digit_sum(periodic_seqs[0]) * 1,
    1: digit_sum(periodic_seqs[1]) * 10,
    2: digit_sum(periodic_seqs[2]) * 100,
    3: digit_sum(periodic_seqs[3]) * 1000,
    4: digit_sum(periodic_seqs[4]) * 10000,
    5: digit_sum(periodic_seqs[5]) * 100000
}

sum_1_n = lambda n: n * (n+1) // 2
# sum x = 1 to n of (10^6)^(x-1) * K
sum_1_n_10_6_S = lambda n, S: (S * ((10**6)**(n%55555) - 1) * 96007682) % 123454321
# sum x = 1 to n of (n+1-x)*(10^6)^(x-1) * K
sum_1_n_M_x_10_6_x_S = lambda n, S: \
(S * ((10**6)**((n+1)%55555) - 999999 * n - 10**6) * 96017284) % 123454321


def get_partial_sum(seqs, n):
    s = 0
    for key, seq in seqs.items():
        s += sum(map(int, seq[:n])) * (10**key)
    return s

if __name__ == "__main__":
    ans = 0

    n_row_periods = N // 15

    n_extra_rows = N % 15

    S = sum(periodic_seq_sums.values())
    ans += sum_1_n_M_x_10_6_x_S(n_row_periods-1, S)
    ans += sum_1_n_10_6_S(n_row_periods, get_partial_sum(periodic_seqs, n_extra_rows))

    S = sum(prefix_seq_sums.values())
    ans += sum_1_n_10_6_S(n_row_periods, S)
    ans += get_partial_sum(prefix_seqs, n_extra_rows) * ((10**6) ** (n_row_periods%55555))

    ans = ans % MOD

    print(ans)

