// Question: https://projecteuler.net/problem=365
// g++ problem365.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -Wl,-rpath $HOME/pari/lib -lpari -O3

/*
  . C(n,k) = n! / k! / (n-k)!
  . To calculate C(n,k) mod p where p is a prime,
    + First, we need to know whether C(n,k) is divisible by p. If it is, C(n,k) mod p = 0
    + If C(n,k) is not divisible by p, we can safely ignore all p factors in C(n,k) during the calculation.
    + From Wilson's theorem, we know that, (p-1)! = -1 (mod p) where p is a prime.
    + Let X(n,p) = (n! / p^k) where n! is divisible by p^k and n! is not divisible by p^(k+1). (i.e. we remove all factor p from n!)
        + We can compute X(n,p) using Wilson's theorem:
            + X(n,p)       = 1 * 2 * 3 * ... * (p-1) * 1 * (p+1) * (p+2) * ... * (2p-1) * 2 * (2p+1) * ... * m * (mp+1) * ... * (mp+r) where m = n/p
            + X(n,p) mod p = 1 * 2 * 3 * ... * (p-1) * 1 *   1   *   2   * ... *   p-1  * 2 *                       1   * ... *   r
                                                      ^^^                                ^^^                ^^^  ^^^^^^^^^^^^^^^^^^^^^
                                                      (1)                                (1)                (1)           (2)
                           
                           = {[(p-1)! mod p]^m} * (r! mod p) * X(m,p)
                                                   ^^^^^^^^    ^^^^^^
                                                   from (1)    from (2)
    
    + Then, we can calculate C(n,k) by C(n,k) mod p = X(n,p) / X(k,p) / X(n-k,p)
    + Knowing C(n,k) mod p for all p where p is a prime in [1000, 5000], we can use Chinese Remainder Theorem to calculate C(n,k) mod (p*q*r)
      where p,q,r are distinct primes in [1000, 5000].
 */

#include<iostream>
#include<fstream>
#include<array>
#include<vector>
#include<unordered_map>
#include<pari/pari.h>
#include<string>
#include<sstream>
#include<algorithm>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp - 1);
}

const ui N = pow(10, 18);
const ui K = pow(10, 9);

string uint128_to_string(ui n)
{
    stringstream stream;

    if (n == 0)
        stream << "0";

    while (n > 0)
    {
        char r = n % 10;
        stream << char('0' + r);
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    return str;
}

ostream& operator<<(ostream& os, const __uint128_t& n)
{
    os << uint128_to_string(n);
    return os;
}


vector<ui> get_primes(ui lowerbound, ui upperbound)
{
    vector<ui> primes;
    
    ifstream input_stream("../inputs/primes_1e6.txt");
    uint64_t prime;
    while (input_stream >> prime)
    {
        if (prime > upperbound)
            break;
        if (prime > lowerbound)
            primes.push_back(prime);
    }
    input_stream.close();

    return primes;
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


ui get_factorial_prime_mod(ui n, ui prime_mod)
{
    ui mod = 1;

    /*
    // Wilson's theorem: (n-1)! = -1 (mod n) where n is prime
    ui exp = n / prime_mod;
    ui r = n % prime_mod;

    if (exp % 2 == 0)
        mod = 1;
    else
        mod = prime_mod - 1;

    for (ui k = 2; k <= r; k++)
    {
        mod *= k;
        mod %= prime_mod;
    }
    */

    ui exp = n / prime_mod;
    ui r = n % prime_mod;
    while (exp > 0)
    {
        //cout << exp << "," << r << endl;
        if (exp % 2 == 0)
            mod *= 1;
        else
            mod *= (prime_mod - 1);
        mod %= prime_mod;

        for (ui k = 2; k <= r; k++)
        {
            mod *= k;
            mod %= prime_mod;
        }

        n = exp;
        exp = n / prime_mod;
        r = n % prime_mod;
    }

    for (ui k = 2; k <= r; k++)
    {
        mod *= k;
        mod %= prime_mod;
    }

    return mod;
}

ui get_binomial_coeff_mod(ui n, ui k, ui prime_mod)
{
    ui numerator = get_factorial_prime_mod(n, prime_mod);
    ui denominator = (get_factorial_prime_mod(k, prime_mod) * get_factorial_prime_mod(n-k, prime_mod)) % prime_mod;

    ui n_prime_mod_factor_numerator = 0;
    ui base = prime_mod;
    while (base <= n)
    {
        n_prime_mod_factor_numerator += n / base;
        base *= prime_mod;
    }

    ui n_prime_mod_factor_denumerator = 0;
    base = prime_mod;
    while (base <= k)
    {
        n_prime_mod_factor_denumerator += k / base;
        base *= prime_mod;
    }
    base = prime_mod;
    while (base <= (n-k))
    {
        n_prime_mod_factor_denumerator += (n-k) / base;
        base *= prime_mod;
    }

    if (n_prime_mod_factor_numerator > n_prime_mod_factor_denumerator)
    {
        //cout << n << " " << k << " " << prime_mod << " " << n_prime_mod_factor_numerator << " " << n_prime_mod_factor_denumerator<< endl;
        return 0;
    }



    ui mod = numerator * gtolong(lift(gmodulo(ginv(stoi(denominator)), stoi(prime_mod))));
    mod %= prime_mod;

    return mod;
}

ui dfs(ui depth, array<ui, 3>& path, const vector<ui>& primes, const ui& n_primes,
       unordered_map<ui, ui>& binomial_coeff_mods)
{
    if (depth == 3)
    {
        ui p = primes[path[0]];
        ui q = primes[path[1]];
        ui r = primes[path[2]];

        GEN crt = Z_chinese(stoi(binomial_coeff_mods[p]),
                            stoi(binomial_coeff_mods[q]),
                            stoi(p),
                            stoi(q));
        crt = Z_chinese(crt, stoi(binomial_coeff_mods[r]), stoi(p*q), stoi(r));

        return gtolong(lift(crt));
    }

    ui ans = 0;
    ui index_lowerbound = 0;
    if (depth > 0)
        index_lowerbound = path[depth-1] + 1;
    for (ui prime_index = index_lowerbound; prime_index < n_primes; prime_index++)
    {
        if (depth == 0)
            cout << prime_index << endl;
        path[depth] = prime_index;
        ans += dfs(depth+1, path, primes, n_primes, binomial_coeff_mods);
    }

    return ans;
}

int main()
{
    ui ans = 0;

    pari_init(5000000000, 2);

    auto primes = get_primes(1000, 5000);
    unordered_map<ui, ui> binomial_coeff_mods;

    for (auto prime: primes)
        binomial_coeff_mods[prime] = get_binomial_coeff_mod(N, K, prime);

    ui n_primes = primes.size();

    array<ui, 3> path;
    ans = dfs(0, path, primes, n_primes, binomial_coeff_mods);

    cout << ans << endl;

    pari_close();

    return 0;
}