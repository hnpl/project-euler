// Problem: https://projecteuler.net/problem=650
// g++ problem650.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -lpari -O3

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<functional>


using namespace std;

#define endl "\n"

typedef int64_t i64;

const i64 N = 20000;
//const i64 N = 5;
const i64 MOD = 1'000'000'007;

vector<i64> get_primes(i64 n)
{
    vector<i64> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    i64 sqrt_n = i64(sqrt(n));
    for (i64 p = 3; p <= sqrt_n; p+=2)
    {
        if (!is_prime[(p-3)/2])
            continue;
        for (i64 np = 3*p; np <= n; np += 2*p)
            is_prime[(np-3)/2] = false;
    }
    primes.push_back(2);
    i64 size = is_prime.size();
    for (i64 p = 0; 2*p+3 <= n; p++)
    {
        if (is_prime[p])
            primes.push_back(2*p+3);
    }
    return primes;
}

i64 modpow(i64 base, i64 pow, i64 mod)
{
    i64 result = 1;
    base = base % mod;
    while (pow > 0)
    {
        if (pow % 2 == 1)
            result = (result * base) % mod;
        pow = pow / 2;
        base = (base * base) % mod;
    }

    return result;
}

void factor(i64 n, const vector<i64>& primes, vector<i64>& bases, vector<i64>& exponents)
{
    if (n <= 1)
        return; 
    i64 i_prime = 0;
    while (n > 1)
    {
        while (n % primes[i_prime] != 0)
            i_prime += 1;
        i64 base = primes[i_prime];
        i64 exponent = 0;
        while (n % base == 0)
        {
            exponent = exponent + 1;
            n = n / base;
        }
        bases.push_back(base);
        exponents.push_back(exponent);
    }       
}

unordered_map<i64, vector<i64>> cached_bases;
unordered_map<i64, vector<i64>> cached_exponents;
void cached_factor(i64 n, const vector<i64>& primes, vector<i64>& bases, vector<i64>& exponents)
{
    if (cached_bases.find(n) == cached_bases.end())
    {
        vector<i64> _bases;
        vector<i64> _exponents;
        factor(n, primes, _bases, _exponents);
        cached_bases[n] = move(_bases);
        cached_exponents[n] = move(_exponents);
    }
    bases = ref(cached_bases[n]);
    exponents = ref(cached_exponents[n]);
}

i64 invmod(i64 base, i64 mod)
{
    // if k is the multiplicative order of a mod M, then a^k = 1 (mod M)
    // -> a * a^(k-1) = 1 (mod M)
    // -> invmod(a, M) = a^(k-1)
    i64 multiplicative_order = gtolong(order(gmodulss(base, mod)));
    return modpow(base, multiplicative_order-1, mod);
};

i64 D(i64 n, const vector<i64>& primes, const vector<i64>& invmods)
{
    i64 ans = 1;
    unordered_map<i64, i64> exponents;
    for (auto p: primes)
    {
        if (p > n)
            break;
        exponents[p] = 0;
    }

    for (i64 k = 2; k <= n; k++)
    {
        i64 k_exponent_coeff = -n-1+2*k;
        vector<i64> k_bases;
        vector<i64> k_exponents;
        cached_factor(k, primes, k_bases, k_exponents);
        for (i64 j = 0; j < k_bases.size(); j++)
        {
            i64 base = k_bases[j];
            i64 exponent = k_exponents[j];
            exponents[base] += exponent * k_exponent_coeff;
        }
    }

    for (auto p: exponents)
    {
        i64 base = p.first;
        i64 exponent = p.second;

        i64 next_coeff = modpow(base, exponent+1, MOD) - 1;
        next_coeff = next_coeff * invmods[base-1]; // a^-1 % MOD
        next_coeff = next_coeff % MOD;
        ans = ans * next_coeff;
        ans = ans % MOD;
    }
    return ans;
}

int main()
{
    i64 ans = 0;


    vector<i64> primes = get_primes(N);
    pari_init(500000000,2);

    vector<i64> invmods = vector<i64>(N+1, 0);
    for (auto p: primes)
        invmods[p-1] = (invmod(p-1, MOD));

    pari_close();
   
    for (i64 i = 1; i <= N; i++)
    {
        ans = ans + D(i, primes, invmods);
        ans = ans % MOD;
    }

    cout << ans << endl;

    return 0;
}
