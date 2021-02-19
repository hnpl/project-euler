// Question: https://projecteuler.net/problem=266

// g++ problem266.cpp -O3 -std=c++17

/*
    Let p be product of all primes < 190.
    Meet-in-the-middle approach:
        - A divisor of p is the product of a subset of the prime set, so for each prime, we make a decision
          of whether a prime belongs to the subset of PSR(p).
        - We can use ln(divisor) instead to avoid integer overflow. (ln = natural logarithm)
          Thus, suppose divisor = prime_1 * prime_2 * ... * prime_n,
          then, ln(divisor) = ln(prime_1) + ln(prime_2) + ... + ln(prime_n)
        - Split the primes into 2 sets, e.g. left and right, each has 21 primes.
        - For the left set, we find all possible values of ln(divisor) where divisor is formed by a subset of prime in the left set.
          Let the sorted list of possible values of ln(divisor) be sorted_left_sum.
        - For each of possible values of ln(divisor) of the right set, we use binary search on sorted_left_sum to find the largest left_sum such that
            . left_sum in sorted_left_sum
            . left_sum + ln(divisor) <= log(p)/2
        - So, compared to iterating through all possible combinations of primes, which takes O(2^n) time,
              meet-in-the-middle takes O(n/2 * 2^(n/2)) time to produce the result.


 */

#include<iostream>
#include<math.h>
#include<vector>
#include<array>
#include<numeric>
#include<algorithm>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

constexpr ui _pow10(ui n)
{
    if (n == 0)
        return 1;
    return _pow10(n-1) * 10;
};

const ui MOD = _pow10(16);

// prime stuff
const ui n_primes = 42;

const array<ui, n_primes> primes = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181};

constexpr array<double, n_primes> get_log_primes(const array<ui, n_primes>& primes)
{
    array<double, n_primes> log_primes = array<double, n_primes>();
    for (ui i = 0; i < n_primes; i++)
        log_primes[i] = log(primes[i]);
    return log_primes;
};

const array<double, n_primes> log_primes = get_log_primes(primes);

constexpr double sum_array(const array<double, n_primes>& arr)
{
    double sum = 0.0;
    for (ui i = 0; i < arr.size(); i++)
        sum += arr[i];
    return sum;
};

const auto log_p = sum_array(log_primes);

// ---

constexpr array<ui, n_primes> get_pow2()
{
    array<ui, n_primes> pow2 = array<ui, n_primes>();
    pow2[0] = 1;
    for (ui i = 1; i < n_primes; i++)
        pow2[i] = pow2[i-1] * 2;
    return pow2;
};
const auto pow2 = get_pow2();

vector<ui> argsort(const vector<double>& vals)
{
    vector<ui> idx = vector<ui>(vals.size());
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(), [&vals](size_t idx1, size_t idx2) {return vals[idx1] < vals[idx2];});
    return idx;
}

void dfs_left(ui depth, const ui& max_depth,
              const array<double, n_primes>& elements,
              double curr_sum, const double& target_sum, ui index,
              vector<double>& logs_of_product)
{
    //if (curr_sum > target_sum)
    //    return;
    if (depth == max_depth)
    {
        logs_of_product[index] = curr_sum;
        return;
    }
    dfs_left(depth+1, max_depth, elements, curr_sum, target_sum, index, logs_of_product);
    dfs_left(depth+1, max_depth, elements, curr_sum + elements[depth], target_sum, index | pow2[depth], logs_of_product);
}

ui binary_search(const vector<double>& elements, const double target) // find the largest x in elements such that x < target
{
    ui l = 0;
    ui r = elements.size();

    ui m = -1ULL;
    while (l <= r)
    {
        m = (l + r) / 2;
        if (elements[m] < target)
            l = m + 1;
        else
            r = m - 1;
    }

    if (elements[m] > target)
        return m - 1;

    return m;
}

void dfs_right(ui depth, const ui& max_depth,
               const array<double, n_primes>& elements,
               double curr_sum, const double& target_sum,
               const vector<double>& logs_left, const vector<ui>& left_indices,
               ui right_index,
               double& curr_max_sum,
               ui& curr_max_sum_left_index, ui& curr_max_sum_right_index)
{
    static const ui offset = n_primes / 2;
    if (depth == max_depth)
    {
        double remaining_sum = target_sum - curr_sum;
        if (remaining_sum < 0)
            return;
        ui best_log_left_index = binary_search(logs_left, remaining_sum);
        if (best_log_left_index == -1ULL) // fail to find a candidate that is smaller than remaining sum
            return;
        curr_sum = logs_left[best_log_left_index] + curr_sum;
        if (curr_sum > curr_max_sum)
        {
            curr_max_sum = curr_sum;
            curr_max_sum_left_index = left_indices[best_log_left_index];
            curr_max_sum_right_index = right_index;
        }
        return;
    }
    dfs_right(depth+1, max_depth, elements, curr_sum,                   target_sum, logs_left, left_indices, right_index,                      curr_max_sum, curr_max_sum_left_index, curr_max_sum_right_index);
    dfs_right(depth+1, max_depth, elements, curr_sum + elements[depth], target_sum, logs_left, left_indices, right_index | pow2[depth-offset], curr_max_sum, curr_max_sum_left_index, curr_max_sum_right_index);
}

int main()
{
    double target_log = log_p / 2;
    vector<double> logs_left;
    logs_left.resize(pow2[n_primes/2]);

    dfs_left(0, n_primes/2, log_primes, 0.0, target_log, 0, logs_left);

    auto sorted_logs_left_index = argsort(logs_left);
    sort(logs_left.begin(), logs_left.end());

    ui ans_left_index = 0;
    ui ans_right_index = 0;
    double max_sum = 0.0;
    dfs_right(n_primes/2, n_primes, log_primes, 0.0, target_log, logs_left, sorted_logs_left_index, 0, max_sum, ans_left_index, ans_right_index);

    ui ans = 1;


    for (ui i = 0; i < n_primes / 2; i++)
    {
        ui bit = ans_left_index & 1;
        ans_left_index >>= 1;
        if (bit == 1)
        {
            ans *= primes[i];
            ans %= MOD;
        }

    }
    
    for (ui i = n_primes/2; i < n_primes; i++)
    {
        ui bit = ans_right_index & 1;
        ans_right_index >>= 1;
        if (bit == 1)
            ans *= primes[i];
            ans %= MOD;
    }

    cout << ans << endl;

    return 0;
}