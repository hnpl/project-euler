// Question: https://projecteuler.net/problem=675

/*
 * Ideas,
 */

#include"pari/pari.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cassert>
#include<sstream>
#include<unordered_map>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return pow(base, exp-1) * base;
};

const ui N = pow(10, 7);
const ui MOD = pow(10, 9) + 87;

ui inverse_mod(const ui n, const ui mod)
{
    ui ans = Fl_inv(n, mod);
    return ans;
}

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

ui calculate_S(const vector<ui>& exponents, const ui& num_valid_entries)
{
    ui ans = 1;
    for (ui i = 0; i < num_valid_entries; i++)
        ans *= (2*exponents[i] + 1);
    return ans;
}

unordered_map<ui, ui> path_to_map(const vector<ui>& prime_path, const vector<ui>& exp_path, const ui& depth)
{
    unordered_map<ui, ui> m;
    for (ui i = 0; i < depth; i++)
        m[prime_path[i]] = exp_path[i];
    return m;
}

void dfs(const ui& depth, vector<ui>& prime_idx_path, vector<ui>& exp_path,
         const int64_t& prev_prime_idx, const int64_t& max_prime_idx,
         const ui& curr_prod, const ui& max_prod, const vector<ui>& primes,
         unordered_map<ui, unordered_map<ui, ui>>& k_exp)
{
    k_exp[curr_prod] = move(path_to_map(prime_idx_path, exp_path, depth));
    if (prev_prime_idx == max_prime_idx)
        return;
    int64_t next_prime_idx = prev_prime_idx + 1;
    for (int64_t next_prime_idx = prev_prime_idx + 1; next_prime_idx < max_prime_idx; next_prime_idx++)
    {
        const ui prime = primes[next_prime_idx];
        ui prime_exp = 1;
        ui next_prod = curr_prod * prime;
        while (next_prod <= max_prod)
        {
            prime_idx_path[depth] = next_prime_idx;
            exp_path[depth] = prime_exp;
            dfs(depth+1, prime_idx_path, exp_path, next_prime_idx, max_prime_idx, next_prod, max_prod, primes, k_exp);
            prime_exp += 1;
            next_prod *= prime;
        }
    }
}

inline ui get_inverse_mod(const ui& n, const ui& mod, unordered_map<ui, ui>& cached_inverse_mod)
{
    if (cached_inverse_mod.find(n) == cached_inverse_mod.end())
        cached_inverse_mod[n] = inverse_mod(n, mod);
    return cached_inverse_mod[n];
}

ui solve(const ui& n, const ui& mod)
{
    ui sqrt_n = ceil(sqrt(n));
    vector<ui> small_primes = get_primes(sqrt_n);
    unordered_map<ui, unordered_map<ui, ui>> k_exp;
    vector<ui> S_n_factorial = vector<ui>(n+1, 0);
    vector<ui> prime_idx_path = vector<ui>(small_primes.size(), 0);
    vector<ui> exp_path = vector<ui>(small_primes.size(), 0);
    unordered_map<ui, ui> cached_inverse_mod;
    vector<ui> k_smooth_numbers; // contains k where k is formed by only small prime factors
    vector<ui> k_big_prime_factor = vector<ui>(n+1, 0); // if a number k has a big prime factor, this array[k] has that prime factor

    // first pass
    dfs(0, prime_idx_path, exp_path, -1, small_primes.size(), 1, n, small_primes, k_exp);

    // second pass
    k_smooth_numbers.reserve(k_exp.size());
    for (auto p: k_exp)
        k_smooth_numbers.push_back(p.first);
    sort(k_smooth_numbers.begin(), k_smooth_numbers.end());
    for (ui i = 2; i <= n; i++)
    {
        if ((k_exp.find(i) != k_exp.end()) || (k_big_prime_factor[i] != 0)) // not a big prime number
            continue;
        const ui big_prime = i;
        for (auto const k_smooth_num: k_smooth_numbers)
        {
            if (big_prime * k_smooth_num > n)
                break; // we sorted k_smooth_num so we can break here
            k_big_prime_factor[big_prime * k_smooth_num] = big_prime;
        }
    }

    // third pass
    //vector<bool> zero_exp = vector<bool>(n+1, true);
    vector<ui> running_k_factorial_exp = vector<ui>(n+1, 0); // contains exponents of factorized k!
    ui running_S_k = 1;
    for (ui k = 2; k <= n; k++)
    {
        unordered_map<ui, ui> k_factorized_exp;
        if (k_big_prime_factor[k] == 0) // k does not contain big prime factors -> we already compute the factorized exponents
        {
            k_factorized_exp = k_exp[k];
        }
        else
        {
            const ui k_smooth_num = k / k_big_prime_factor[k];
            k_factorized_exp = k_exp[k_smooth_num];
            //k_factorized_exp[k_big_prime_factor[k]] = 1;
        }

        for (auto p: k_factorized_exp)
        {
            const ui prime_factor = small_primes[p.first];
            const ui prime_exp = p.second;
            const ui curr_prime_exp = running_k_factorial_exp[prime_factor];
            const ui next_prime_exp = curr_prime_exp + prime_exp;
            running_S_k *= get_inverse_mod(2*curr_prime_exp+1, mod, cached_inverse_mod);
            running_S_k %= mod;
            running_S_k *= (2*next_prime_exp+1);
            running_S_k %= mod;
            running_k_factorial_exp[prime_factor] = next_prime_exp;
        }

        if (k_big_prime_factor[k] != 0) // account for the big prime factor
        {
            const ui big_prime_factor = k_big_prime_factor[k];
            const ui big_prime_exp = 1;
            const ui curr_big_prime_exp = running_k_factorial_exp[big_prime_factor];
            const ui next_big_prime_exp = curr_big_prime_exp + big_prime_exp;
            running_S_k *= get_inverse_mod(2*curr_big_prime_exp+1, mod, cached_inverse_mod);
            running_S_k %= mod;
            running_S_k *= (2*next_big_prime_exp+1);
            running_S_k %= mod;
            running_k_factorial_exp[big_prime_factor] = next_big_prime_exp;
        }

        S_n_factorial[k] = running_S_k;
//        if (k <= 100)
//        {
//            cout << k << " " << running_S_k << endl;
//        }

/*
        cout << k << " = ";
        for (auto p: k_factorized_exp)
            cout << small_primes[p.first] << "^" << p.second << " ";
        if (k_big_prime_factor[k] != 0)
            cout << k_big_prime_factor[k] << "^" << 1;
        cout <<"; S[k!] = " << S_n_factorial[k];
        cout << endl;
*/
    }

    ui ans = 0;
    for (auto const& e: S_n_factorial)
    {
        //assert(e != 0);
       // cout << e << endl;
        ans += e;
        ans %= mod;
    }
    return ans;
}

int main()
{
    ui ans = solve(N, MOD);
    cout << ans << endl;
    return 0;
}

