// Problem: https://projecteuler.net/problem=171
// g++ problem171.cpp -O3 --std=c++2a

/*
    Strategy:
        - The maximum sum of square of 20 digits is 20 * (9^2) = 1620
                -> there are 40 different possibilities for the sum (e.g. 1, 4, 9, 16, 25, etc.)
        - It's fast to generate a monotonically sequence of 20 digits that sums up to a certain number.
                -> generate such sequences that each sequence sums up to one of the above square numbers.
        - For each of sequence, the distribution of digits in the sequence is different than that of other sequences.
                -> we can permutate each sequence and find the sum.
        - Since we only need the sum of permutations, we can exploit the "symmetry" of permutations of a certain set.
                -> we don't need to iterate through all the permutations!
                -> how to find the sums?
                        -> first, we find the number of unique permutations of the set where the order of number matters
                                -> this can be done using multinomial coefficient (https://mathworld.wolfram.com/MultinomialCoefficient.html)
                                        -> for a set of x1 object 1, x2 object 2, ..., xm object m, the number of permutations where the order matters is
                                                factorial(x1+x2+...+xm) / [factorial(x1) * factorial(x2) * ... * factorial(xm)]
                        -> second, let's lay the permutation vertically. Let say we have a set {a,a,b,b,c},
                           then, we have
                                {a, a, b, b, c}
                                {a, a, b, c, b}
                                {a, a, c, b, b}
                                {a, b, a, b, c}
                                {a, b, a, c, b}
                                {a, b, b, a, c}
                                {a, b, b, c, a}
                                {a, b, c, a, b}
                                {a, b, c, b, a}
                                {a, c, a, b, b}
                                {a, c, b, a, b}
                                {a, c, b, b, a}
                                {b, a, a, b, c}
                                {b, a, a, c, b}
                                {b, a, b, a, c}
                                {b, a, b, c, a}
                                {b, a, c, a, b}
                                {b, a, c, b, a}
                                {b, b, a, a, c}
                                {b, b, a, c, a}
                                {b, b, c, a, a}
                                {b, c, a, a, b}
                                {b, c, a, b, a}
                                {b, c, b, a, a}
                                {c, a, a, b, b}
                                {c, a, b, a, b}
                                {c, a, b, b, a}
                                {c, b, a, a, b}
                                {c, b, a, b, a}
                                {c, b, b, a, a}
                        Let a, b, c be the digits and we want to find the sum of the numbers formed by the digits.
                        Note that, each column has the same distribution of a, b, c.
                        This leads to a couple of nice properties:
                            -> each column has the same sum
                            -> we know the distribution of digits of each column (|a|:|b|:|c| = 2:2:1 in this case) (1)
                            -> we know the number of digits of each colomn (= number of permutations) (2)
                            -> from (1) and (2), we can figure out how many each digit appears in a column, and from this, we can find the sum of each column
                            -> we can find the sum of all permutations without having to iterate through all permutations.
*/

#include<iostream>
#include<vector>
#include<unordered_map>
#include<numeric>
#include<algorithm>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 20;

const ui M = 9;

const ui MOD = 1'000'000'000ULL;

unordered_map<uint64_t, ui> squares_map = {{0, 0}, {1, 1}, {4, 2}, {9, 3}, {16, 4},
                                           {25, 5}, {36, 6}, {49, 7}, {64, 8}, {81, 9}};

constexpr ui factorial(ui n)
{
    if (n == 0)
        return 1;
    else
        return n * factorial(n-1);
}

ui calculate_all_permutation_mod(vector<ui> digits)
{
    vector<ui> digit_freq = vector<ui>(10, 0);
    for (ui i = 1; i <= N; i++)
        digit_freq[digits[i]] = digit_freq[digits[i]] + 1;

    // Multinomial coeff C(N, (freq1, freq2, ...))
    ui n_permutations = factorial(20);
    for (auto freq: digit_freq)
        n_permutations = n_permutations / factorial(freq);

    // Reduce the ratio so that n_permutations is divisible by the sum of the ratio
    ui GCD = digit_freq[0];
    for (ui i = 1; i <= 9; i++)
        GCD = gcd(GCD, digit_freq[i]);

    for (ui i = 0; i <= 9; i++)
        digit_freq[i] = digit_freq[i] / GCD;

    ui freq_sum = 0;
    for (auto freq: digit_freq)
        freq_sum = freq_sum + freq;

    vector<ui> digit_freq_per_column = vector<ui>(10, 0);
    for (ui i = 0; i < digit_freq_per_column.size(); i++)
        digit_freq_per_column[i] = n_permutations / freq_sum * digit_freq[i];
    
    ui column_sum = 0;
    for (ui i = 1; i < digit_freq_per_column.size(); i++)
        column_sum = column_sum + digit_freq_per_column[i] * i;
    
    ui mod = 0;
    ui coeff = 1;
    column_sum = column_sum % MOD;
    for (ui i = 0; i < M; i++)
    {
        mod = mod + column_sum * coeff;
        mod = mod % MOD;
        coeff = coeff * 10;
    }
    return mod;
}


ui DFS(ui depth, ui curr_sum, const ui& target, vector<ui>& digits)
{
    if (depth == N)
    {
        ui digit_square = target - curr_sum;
        if (squares_map.find(digit_square) != squares_map.end())
        {
            digits[depth] = squares_map[digit_square];
            if (digits[depth] < digits[depth-1])
                return 0;
            ui total = calculate_all_permutation_mod(digits);
            return total;
        }
        return 0;
    }

    ui total = 0;

    for (ui digit = digits[depth-1]; digit <= 9; digit++)
    {
        ui next_sum = curr_sum + digit * digit;
        digits[depth] = digit;
        if (next_sum > target)
            break;
        ui max_remaining_sum = (N-depth) * (9*9);
        if (max_remaining_sum < target - next_sum)
            continue;
        total = total + DFS(depth + 1, next_sum, target, digits);
        total = total % MOD;
    }

    return total;
}

int main()
{
    ui ans = 0;

    ui max_sum_of_digit_squares = N * (9*9);

    vector<ui> valid_sums;
    for (ui i = 1; i*i <= max_sum_of_digit_squares; i++)
        valid_sums.push_back(i*i);

    vector<ui> digits = vector<ui>(N+1);
    digits[0] = 0;
    for (auto& valid_sum: valid_sums)
    {
        ans = ans + DFS(1, 0, valid_sum, digits);
        ans = ans % MOD;
    }

    cout << ans << endl;

    return 0;
}


