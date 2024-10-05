// Question: https://projecteuler.net/problem=853

/*
 * https://en.wikipedia.org/wiki/Pisano_period#Properties
 * . Important properties,
 *   . Except for pi(2) = 3, pi(n) is even for all n other than 2.
 *   . If gcd(m,n) = 1, then pi(m*n) = pi(m)*pi(n)
 *   . Here we assume that pi(p^k) = p^(k-1) * pi(p) for all prime p. This has not been proven to be true.
 * . We're targetting pi(n) = 120.
 * . Assume that n = (p_1 ^ k_1) * (p_2 ^ k_2) * ... * (p_m ^ k_m) for some distinct primes p_1, p_2, ..., p_m.
 *   Then pi(n) = [p_1^(k_1-1) * pi(p_1)] * [p_2^(k_2-1) * pi(p_2)] * ... * [p_m^(k_m-1) * pi(p_m)].
 *   Since pi(n) = 120, each of {pi(p_1), pi(p_2), ..., pi(p_m)} must be a divisor of 120.
 *   So, we find all prime p such that pi(p) divides 120, then use those primes to construct n.
 */

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>

using namespace std;

typedef uint64_t ui;

const ui N = 1000000000;
const ui M = 120;
// Pisano_period(p) is even except for p = 2
const vector<ui> M_divisors_of_interest = { 2, 3, 4, 6, 8, 10, 12, 20, 24, 30, 40, 60, 120 };
const unordered_set<ui> M_divisors_of_interest_set = unordered_set<ui>(M_divisors_of_interest.begin(), M_divisors_of_interest.end());

vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(n));
    for (ui p = 3; p <= sqrt_n; p+=2)
    {
        if (!is_prime[(p-3)/2])
            continue;
        for (ui np = 3*p; np <= n; np += 2*p)
            is_prime[(np-3)/2] = false;
    }
    primes.push_back(2);
    ui size = is_prime.size();
    for (ui p = 0; 2*p+3 <= n; p++)
    {
        if (is_prime[p])
            primes.push_back(2*p+3);
    }
    return primes;
}

array<ui, 2*M> generate_first_2M_fibonacci_numbers_mod_n(const ui& mod)
{
    array<ui, 2*M> fib;

    fib[0] = 1;
    fib[1] = 1;
    for (ui i = 2; i < 2*M; i++)
        fib[i] = (fib[i-2] + fib[i-1]) % mod;

    return fib;
}

// return true if the period is `period` given the first 2M Fibonacci numbers
bool verify_Pisano_period(const array<ui, 2*M>& fib, const ui& period)
{
    for (ui i = 0; i < period; i++)
        if (fib[i] != fib[i+period])
            return false;
    return true;
}

// return 0 if the pisano_period(n) is in the M_divisors set
ui find_Pisano_period(const ui& n)
{
    array<ui, 2*M> fib_mod_n = generate_first_2M_fibonacci_numbers_mod_n(n);
    for (auto const& div: M_divisors_of_interest)
        if (verify_Pisano_period(fib_mod_n, div))
            return div;
    return 0;
}

// return a pair of (n, Pisano_period(n)) such that,
//   . Pisano_period(n) is in M_divisors_of_interest
vector<pair<ui, ui>> find_seeds()
{
    vector<pair<ui, ui>> seeds;
    seeds.reserve(10);
    //vector<ui> primes = get_primes(int(sqrt(N)));
    vector<ui> primes = get_primes(N);
    for (auto const& prime: primes)
    {
        ui Pisano_period = find_Pisano_period(prime);
        if (Pisano_period != 0)
            seeds.push_back(move(make_pair(prime, Pisano_period)));
    }
    return seeds;
}

void find_n_with_Pisano_period(
    const vector<pair<ui, ui>>& seeds, const ui& depth, const ui& max_depth,
    const ui& current_number, const ui& current_Pisano_period, const ui& target_Pisano_period,
    vector<ui>& ans
) // aka doing DFS
{
    if (depth == max_depth)
        return;

    if (current_number >= N)
        return;

    for (ui next_depth = depth+1; next_depth < max_depth; next_depth++)
    {
        ui base_prime = seeds[next_depth].first;
        ui Pisano_period_factor = seeds[next_depth].second; // Pisano_period_factor contributed by base_prime**k
        ui next_number = current_number * base_prime;
        while (next_number < N)
        {
            ui next_Pisano_period = lcm(current_Pisano_period, Pisano_period_factor);
            if (next_Pisano_period > M)
                break;
            if (next_Pisano_period == target_Pisano_period)
                ans.push_back(next_number);
            if (M_divisors_of_interest_set.find(next_Pisano_period) != M_divisors_of_interest_set.end())
                find_n_with_Pisano_period(seeds, next_depth, max_depth, next_number, next_Pisano_period, target_Pisano_period, ans);
            next_number *= base_prime;
            Pisano_period_factor *= base_prime;
        }
    }
}

int main()
{
    // find seeds
    auto seeds = find_seeds();
    for (auto p: seeds)
        cout << p.first << " " << p.second << endl;

    // find all n where Pisano_period(n) == M
    vector<ui> ans;
    ans.reserve(100);
    find_n_with_Pisano_period(seeds, -1, seeds.size(), 1, 1, M, ans);

    sort(ans.begin(), ans.end());

    // find sum of all n
    ui sum = 0;
    for (auto const& n: ans)
    {
        cout << n << endl;
        sum += n;
    }
    cout << sum << endl;

    return 0;
}
