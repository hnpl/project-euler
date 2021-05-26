#include<iostream>
#include<vector>
#include<array>
#include<cmath>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp - 1);
}

const ui N = pow(10, 8);
//const ui N = 10;
const ui MOD = pow(10, 9) + 7;


vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(float(n)));
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

ui count_coprimes(const ui& n_prime_factors, const array<ui, 9>& prime_factors, const ui& upperbound)
{
    ui n_non_coprimes = 0;

    // principle of inclusion-exclusion
    ui n_combinations = pow(2, n_prime_factors);
    for (ui combination = 1; combination < n_combinations; combination++)
    {
        ui n_bits_set = 0;
        ui index = 0;
        ui product = 1;
        ui c = combination;
        while (c > 0)
        {
            if (c % 2 == 1)
            {
                product *= prime_factors[index];
                n_bits_set += 1;
            }
            index++;
            c >>= 1;
        }
        if (n_bits_set % 2 == 0)
            n_non_coprimes -= upperbound / product;
        else
            n_non_coprimes += upperbound / product;
    }

    return upperbound - n_non_coprimes;
}

ui powmod(ui base, ui exp, const ui& mod)
{
    ui ans = 1;
    base = base % mod;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            ans = (ans * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return ans;
}

void dfs2(const ui& depth, const ui& max_depth, const array<ui, 9>& path, const ui& curr_product, const ui& n_coprimes_of_N, ui& ans)
{
    if (depth == max_depth)
    {
        ans *= ((powmod(curr_product, n_coprimes_of_N - count_coprimes(depth, path, curr_product), MOD)) % MOD);
        ans %= MOD;
        return;
    }
    ui curr_prime = path[depth];
    ui next_product = curr_product;
    for (ui exp = 1; ; exp++)
    {
        next_product *= curr_prime;
        if (next_product > N)
            return;
        dfs2(depth+1, max_depth, path, next_product, n_coprimes_of_N, ans);
    }
}

void dfs(const ui& depth, const ui& max_depth,
         array<ui, 9>& path, const ui& curr_product,
         const vector<ui>& primes, const ui& n_primes, const ui& curr_prime_index, ui& ans)
{
    if (curr_product > N)
        return;

    ui n_coprimes_of_N = count_coprimes(depth, path, N);
    dfs2(0, depth, path, 1, n_coprimes_of_N, ans);

    for (ui next_prime_index = curr_prime_index + 1; next_prime_index < n_primes; next_prime_index++)
    {
        ui next_prime = primes[next_prime_index];
        path[depth] = next_prime;
        ui next_product = curr_product * next_prime;
        if (next_product > N)
            break;
        dfs(depth+1, max_depth, path, next_product, primes, n_primes, next_prime_index, ans);
    }
}

int main()
{
    ui ans = 1;

    auto primes = get_primes(N);
    ui n_primes = primes.size();

    array<ui, 9> path;
    for (ui starting_prime_index = 0; starting_prime_index < n_primes; starting_prime_index++)
    {
        ui starting_prime = primes[starting_prime_index];
        path[0] = starting_prime;
        dfs(1, 9, path, starting_prime, primes, n_primes, starting_prime_index, ans);
    }
    cout << ans << endl;

    return 0;
}
