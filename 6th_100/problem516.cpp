// Question: https://projecteuler.net/problem=516

/*
 * Ideas,
 *   - euler_phi(n) = n prod_{prime factor p of n} (1-1/p)
 *                  = p_1^(a_1 - 1) * (p_1 - 1) * p_2^(a_2 - 1) * (p_2 - 1) * ... * p_m^(a_m - 1) * (p_m - 1)
 *                  where p_i are primes.
 *   - Since we assume euler_phi(n) to be 5-smooth, it means p_i^(a_i-1) is 5-smooth for all i, and (p_i - 1) must also be 5-smooth for all i.
 *   - So, we can start from generating all 5-smooth number k such that k + 1 is a prime number.
 *     Then, we plug all the primes to generate n = p_1^a_1 * p_2^a_2 * ... * p_m^a_m.
 *     Note that, for all i, we want p_i^(a_i-1) to be 5-smooth. So,
 *       - If p_i = 2 or p_i = 3 or p_i = 5, then a_i can be any integers >= 1.
 *       - For other primes p_i, then a_i = 1 (this is because, if a_i > 1, then p_i^(a_i-1) is not 5-smooth).
 *   - Runtime: ~4mins
 */
#include"pari/pari.h"
#include<algorithm>
#include<array>
#include<cassert>
#include<iostream>
#include<sstream>
#include<vector>

using namespace std;

typedef uint64_t ui;

constexpr ui pow(const ui& base, const ui& exp)
{
    if (base == 0)
        return 1;
    return base * pow(base, exp - 1);
}

const ui N = pow(10, 12);
const ui MOD = pow(2, 32);

struct FiveSmoothNumber
{
    ui val;
    FiveSmoothNumber() { val = -1; }
    FiveSmoothNumber(const ui& _val)
    {
        val = _val;
    }
    inline FiveSmoothNumber multiplyWith(const FiveSmoothNumber& other) const
    {
        return FiveSmoothNumber(val * other.val);
    }
    inline ui getPrime() const
    {
        return val + 1;
    }
};

bool isprime(ui n)
{
    return ispseudoprime(utoi(n), 0);
}

void dfs_gen_5_smooth_nums(const ui& depth, const ui& max_depth, const array<ui, 3>& factors,
                           const ui& curr_prod, const ui& max_prod, vector<FiveSmoothNumber>& ans)
{
    ans.push_back(FiveSmoothNumber(curr_prod));
    if (depth == max_depth)
        return;
    for (ui next_depth = depth + 1; next_depth < max_depth; next_depth++)
    {
        const ui factor = factors[next_depth];
        ui next_prod = curr_prod * factor;
        while (next_prod <= max_prod)
        {
            dfs_gen_5_smooth_nums(next_depth, max_depth, factors, next_prod, max_prod, ans);
            next_prod *= factor;
        }
    }
}

ui dfs_gen_and_sum_answer(const ui& depth, const ui& max_depth, const vector<ui>& prime_factors,
                          const ui& curr_prod, const ui& max_prod)
{
    if (depth == max_depth)
    {
        //cout << curr_prod << endl;
        return curr_prod % MOD;
    }
    ui ans = 0;
    ui prime_factor = prime_factors[depth];
    ui next_prod = curr_prod;
    double prime_factor_log = log(prime_factor);
    double next_prod_log = log(next_prod);
    const double max_prod_log = log(max_prod);
    while ((next_prod_log + prime_factor_log) <= max_prod_log)
    {
        next_prod_log += prime_factor_log;
        next_prod *= prime_factor;
        ans += dfs_gen_and_sum_answer(depth+1, max_depth, prime_factors, next_prod, max_prod);
        ans %= MOD;
        if ((prime_factor != 2) && (prime_factor != 3) && (prime_factor != 5))
            break;
    }
    return ans;
}

ui dfs(const ui& depth, const ui& max_depth, vector<FiveSmoothNumber>& totient_factor_path,
       const ui& curr_smooth_number_idx, const vector<FiveSmoothNumber>& smooth_numbers,
       const FiveSmoothNumber& curr_prod, const ui& max_prod)
{
    ui ans = 0;

/*
    if (depth > 0)
    {
        cout << "Considering: (";
        for (ui i = 0; i < depth; i++)
            cout << totient_factor_path[i].getPrime() << ",";
        cout << ")" << endl;
    }
*/

    if (depth > 0)
    {
        vector<ui> prime_factors = vector<ui>(depth);
        for (ui i = 0; i < depth; i++)
            prime_factors[i] = totient_factor_path[i].getPrime();
        ans += dfs_gen_and_sum_answer(0, depth, prime_factors, 1, max_prod);
        ans %= MOD;
    }

    if (depth == max_depth)
        return ans;

    const ui num_smooth_numbers = smooth_numbers.size();
    for (ui next_smooth_number_idx = curr_smooth_number_idx + 1;
         next_smooth_number_idx < num_smooth_numbers;
         next_smooth_number_idx++)
    {
        const FiveSmoothNumber& next_smooth_number = smooth_numbers[next_smooth_number_idx];
        FiveSmoothNumber next_prod = next_smooth_number.multiplyWith(curr_prod);
        if (next_prod.val > max_prod)
            continue;
        totient_factor_path[depth] = next_smooth_number;
        ans += dfs(depth+1, max_depth, totient_factor_path, next_smooth_number_idx, smooth_numbers, next_prod, max_prod);
        ans %= MOD;
    }
    return ans;
}

ui solve(const ui& n)
{
    ui ans = 0;
    const array<ui, 3> factors = {2, 3, 5};
    vector<FiveSmoothNumber> smooth_numbers_candidates;
    dfs_gen_5_smooth_nums(-1, factors.size(), factors, 1, n, smooth_numbers_candidates);
    vector<FiveSmoothNumber> smooth_numbers;
    for (auto const e: smooth_numbers_candidates)
        if (isprime(e.val+1))
            smooth_numbers.push_back(e);
    vector<FiveSmoothNumber> path = vector<FiveSmoothNumber>(smooth_numbers.size());
    ans = dfs(0, path.size(), path, -1, smooth_numbers, 1, n);
    ans += 1; // 1 is valid but is not generated by dfs
    return ans;
}

int main()
{
    pari_init(50000000, 2);
    ui ans = solve(N);
    cout << ans << endl;
    pari_close();
    return 0;
}
