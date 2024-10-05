// Problem: https://projecteuler.net/problem=172

/*
    - Let multinomial(k) apply the multinomial theorem on array k, i.e. C(n, k1, k2, ...)
    - Generate all array A such that: len(A) = 10, sum(A) = 18, A[i] <= 3 for all i, A[i] <= A[j] for all i < j.
    - For each such an array A,
        . Let B be an array such that B[i] = frequency of i in A.
        . Let f be a one-to-one map from each digit to a value in A.
            + Let f(i)=j mean digit i appears j times in the 18-digit number.
            + Cardinality of f is ||f|| = multinomial(B).
            + For each mapping f, the number of distinct 18-digit numbers (with a leading zero) having f as its
histogram of its digit is multinomial(A).
        => for each A, we can generate K = multinomial(A) * multinomial(B) numbers having its digits histogram similar to A.
        . To find the number of numbers having its digits histogram similar to A but with leading zero,
            + For each i in {0, 1, 2, 3}
                + subtract ONE element j in A such that A[j] = i, calling the new array newA.
                + find newB that is corresponding to newA.
                + subtract multinomial(newA) * multinomial(newB) from K.
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N = 18;          // number of digits
const i64 M = 3;           // maximum number of occurrences of each digit
const i64 DIGIT_BASE = 10; // DIGIT_BASE = 10 -> base 10 numbers

constexpr i64 factorial(i64 n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n-1);
};

i64 multinomial(vector<i64> k)
{
    i64 n = accumulate(k.begin(), k.end(), 0);
    i64 ans = factorial(n);
    for (auto i: k)
        ans = ans / factorial(i);
    return ans;
}

i64 calculate_permutations(vector<i64> distribution)
{
    i64 ans = 0;

    i64 n = accumulate(distribution.begin(), distribution.end(), 0);

    i64 distribution_range_lowerbound = *min_element(distribution.begin(), distribution.end());
    i64 distribution_range_upperbound = *max_element(distribution.begin(), distribution.end());

    vector<i64> freq = vector<i64>(distribution_range_upperbound+1, 0);
    for (auto f: distribution)
        freq[f] += 1;

    // find the number of distinct ways of matching each digit to a frequency
    i64 n_unique_dicts = multinomial(freq);

    // for each matching of {digits -> frequencies}, find the number of distinct ways of arranging the digits based on the frequency
    i64 n_ways_per_dict = multinomial(distribution);

    ans = n_unique_dicts * n_ways_per_dict;

    // filter out numbers that start with a zero
    // fix the first number to be zero, and find the number of ways to fill in the rest
    for (i64 f = 1; f <= distribution_range_upperbound; f++)
    {
        if (freq[f] == 0)
            continue;
        freq[f] -= 1;
        n_unique_dicts = multinomial(freq);
        i64 f_index = 0;
        while (distribution[f_index] != f)
            f_index += 1;
        distribution[f_index] -= 1;
        i64 n_ways_per_dict = multinomial(distribution);
        distribution[f_index] += 1;
        freq[f] += 1;
        ans -= n_unique_dicts * n_ways_per_dict;
    }

    return ans;
}

// Find all ways to make a sequence of 10 elements where each element <= 3 and sums up to 18
i64 DFS(i64 digit, vector<i64>& path, i64 curr_sum, const i64& element_upperbound, const i64& target_sum)
{
    i64 total = 0;

    if (digit == DIGIT_BASE - 1)
    {
        path[digit] = target_sum - curr_sum;
        if (path[digit] < 0 || path[digit] < path[digit-1] || path[digit] > element_upperbound)
            return 0;
        total = calculate_permutations(path);
    }
    else
    {
        i64 next_digit = digit + 1;
        i64 element_lowerbound = 0;
        if (digit > 0)
            element_lowerbound = path[digit - 1];
        element_lowerbound = max(element_lowerbound, target_sum - curr_sum - ((DIGIT_BASE) - next_digit)*3);
        for (i64 element = element_lowerbound; element <= min(element_upperbound, target_sum - curr_sum); element++)
        {
            path[digit] = element;
            i64 next_sum = curr_sum + element;
            if (next_sum > target_sum)
                break;
            total += DFS(next_digit, path, next_sum, element_upperbound, target_sum);
        }
    }

    return total;
}

int main()
{
    i64 ans = 0;

    vector<i64> path = vector<i64>(DIGIT_BASE, 0);

    ans = DFS(0, path, 0, M, N);

    cout << ans << endl;

    return 0;
}
