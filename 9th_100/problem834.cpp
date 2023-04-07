// Question: https://projecteuler.net/problem=834

/*
 * Strategy:
 *   . Assume that n is fixed.
 *   . The m^th term of the series is A(m) = n + (n+1) + (n+2) + ... + (n+m) = (m+1)n + m(m+1)/2
 *   . We want to find `m` such that (n+m) | A(m).
 *     . We have that, A(m) (mod n+m) = (m+1)n + m(m+1)/2                      (mod n+m)
 *                                    = (2mn + 2n + m^2 + m)/2                 (mod n+m)
 *                                    = [m(m+n) + (m+n) + mn + n]/2            (mod n+m)
 *                                    = [(m+1)(m+n) + mn + n^2 - n^2 + n]/2    (mod n+m)
 *                                    = [(m+n)(m+1+n) - n(n-1)]/2              (mod n+m)
 *                                    = n(n-1)/2                               (mod n+m)
 *     . This means, (n+m) | n(n-1)/2.
 *       We can find (n+m) by finding all divisors of n(n-1), then for each divisor d, we have m = d - n.
 *       Then we plug m to A(m) and check if (n+m) | A(m).
 *     . We can generate divisors of n(n-1) by generating all prime factors of n and all prime factors of n-1.
 *       Because n and n-1 are co-prime, they have distinct sets prime factors, and thus distinct sets of divisors.
 *       We can generate divisors of n (or n-1) by doing prime factorization, then use dfs to find of combinations of prime factors and theirs exponents.
 *       We can iterate through all divisors of n(n-1) using,
 *          for d1 in divisors(n):
 *              for d2 in divisors(n-1):
 *                  possible_divisor = d1 * d2
 */

#include<iostream>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<string>
#include<sstream>

using namespace std;

typedef uint64_t ui;

const ui N = 1234567;

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

/*
vector<ui> get_sums_of_divisors(ui n)
{
    vector<ui> ans(n+1, 1);
    vector<ui> primes = get_primes(n);
    for (auto prime: primes)
    {
        vector<ui> factor(n+1, 1);
        ui prime_pow = prime;
        while (prime_pow <= n)
        {
            for (ui prime_pow_multiple = prime_pow; prime_pow_multiple <= n; prime_pow_multiple += prime_pow)
                factor[prime_pow_multiple] += prime_pow;
            prime_pow *= prime;
        }
        for (ui i = prime; i <= n; i+=prime)
            ans[i] *= factor[i];
    }
    return ans;
}
*/

vector<unordered_map<ui, ui>> to_prime_factors(ui n)
{
    vector<unordered_map<ui, ui>> ans = vector<unordered_map<ui, ui>>(n+1, unordered_map<ui, ui>());

    vector<ui> primes = get_primes(n);
    for (auto prime: primes)
    {
        ui prime_pow = prime;
        ui pow = 1;
        while (prime_pow <= n)
        {
            for (ui prime_pow_multiple = prime_pow; prime_pow_multiple <= n; prime_pow_multiple += prime_pow)
                ans[prime_pow_multiple][prime] = pow;
            prime_pow *= prime;
            pow += 1;
        }
    }

    // the unfilled ones are prime numbers
    for (ui k = 2; k <= n; k++)
        if (ans[k].size() == 0)
            ans[k][k] = 1;

    return ans;
}

string prime_factors_to_string(const unordered_map<ui, ui>& f)
{
    stringstream s;
    for (auto const & p: f)
        s << p.first << "**" <<p.second << " + ";
    return s.str();
}

ui divisor_sum_from_prime_factors(const unordered_map<ui, ui>& f)
{
    ui ans = 1;
    for (auto const & p: f)
    {
        ui base = p.first;
        ui exp = p.second;
        ans *= (pow(base, exp+1) - 1) / (base - 1);
    }

    return ans;
}

ui divisor_count_from_prime_factors(const unordered_map<ui, ui>& f)
{
    ui ans = 1;
    for (auto const & p: f)
        ans *= (p.second + 1);
    return ans;
}

bool check_solution(const ui& _m, const ui& _n)
{
    __uint128_t m = _m;
    __uint128_t n = _n;
    __uint128_t prod = m*n + n + m*(m+1)/2;
    if (prod % (n + m) == 0)
        return true;
    return false;
}

// - generate all possible divisors from v1 and v2. We can decouple factors contributed by v1 and v2 by having 2 dfs, though it's not done that way here.
// - if a possible divisor is above the threshold, we will check if the solution is valid
// - phase 1 means we are keep increasing the exponent of the current prime factor of v1
// - phase 2 means we no longer update the prime factor contributed by v1, and we only update the current prime factor of v2
// - at each step (not phase), there are four possibilities:
//   - increase the exponent of the current prime factor and keep the same phase.
//   - increase the exponent of the current prime factor and move to the next phase.
//   - change the current prime factor to the next one and keep the same phase.
//   - change the current prime factor to the next one and move to the next phase.
ui dfs(ui idx1, ui idx2, ui idx1_exp, ui idx2_exp, ui curr_prod, const ui& n,
       const vector<pair<ui, ui>>& v1, const vector<pair<ui, ui>>& v2, const ui& threshold, const ui& phase)
{
    ui sum = 0;
    if ((curr_prod > threshold) && check_solution(curr_prod-n, n))
        sum += curr_prod - n;

    if (phase == 1)
    {
        // changing f1
        // . increasing idx1_exp
        if (idx1_exp < v1[idx1].second)
        {
            sum += dfs(idx1, idx2, idx1_exp+1, idx2_exp, curr_prod * v1[idx1].first, n, v1, v2, threshold, 1);
            // choosing the first prime factor contributed by v2
            for (ui first_idx2 = 0; first_idx2 < v2.size(); first_idx2++)
                sum += dfs(idx1, first_idx2, idx1_exp+1, 1, curr_prod * v1[idx1].first * v2[first_idx2].first, n, v1, v2, threshold, 2);
        }
        // . increasing idx1
        for (ui next_idx1 = idx1 + 1; next_idx1 < v1.size(); next_idx1++)
        {
            sum += dfs(next_idx1, idx2, 1, idx2_exp, curr_prod * v1[next_idx1].first, n, v1, v2, threshold, 1);
            // choosing the first prime factor contributed by v2
            for (ui first_idx2 = 0; first_idx2 < v2.size(); first_idx2++)
                sum += dfs(next_idx1, first_idx2, 1, 1, curr_prod * v1[next_idx1].first * v2[first_idx2].first, n, v1, v2, threshold, 2);
        }
    }
    else if (phase == 2)
    {
        // changing f2
        // . increasing idx2_exp
        if (idx2_exp < v2[idx2].second)
            sum += dfs(idx1, idx2, idx1_exp, idx2_exp+1, curr_prod * v2[idx2].first, n, v1, v2, threshold, 2);
        // . increasing idx2
        for (ui next_idx2 = idx2 + 1; next_idx2 < v2.size(); next_idx2++)
            sum += dfs(idx1, next_idx2, idx1_exp, 1, curr_prod * v2[next_idx2].first, n, v1, v2, threshold, 2);
    }
    return sum;
}

// given 2 set of prime factors of 2 co-prime numbers, i.e., f1 is prime factors of n, f2 is prime factors of m, and gcd(n,m)=1
// find the sum all divisors of n*m that are higher than n such that m*n+n+m*(m+1)/2 is divisible by (n+m).
ui sum_divisors_higher_than_threshold(const unordered_map<ui, ui>& f1, const unordered_map<ui, ui>& f2, const ui& n, const ui& threshold)
{
    vector<pair<ui, ui>> v1(f1.begin(), f1.end());
    vector<pair<ui, ui>> v2(f2.begin(), f2.end());
    ui ans = dfs(0, 0, 0, 0, 1, n, v1, v2, threshold, 1) + dfs(0, 0, 0, 0, 1, n, v1, v2, threshold, 2);
    return ans;
}

ui solve(const ui& n)
{
    auto factors = to_prime_factors(n);
    ui ans = 0;
    for (ui k = 3; k <= n; k++)
        ans += sum_divisors_higher_than_threshold(factors[k], factors[k-1], k, k);
    return ans;
}

int main()
{
    ui ans = solve(N);
    cout << ans << endl;
    return 0;
}
