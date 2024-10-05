// Question: https://projecteuler.net/problem=659

/*
 * Ideas,
 *  - We have that if P(k) = p, then
 *         n^2 + k^2 = 0 (mod p) [1]
 *     and (n+1)^2 + k^2 = 0 (mod p) -> n^2 + 2n + 1 + k^2 = 0 (mod p) [2]
 *  - Substract [1] from [2], we have that
 *         2n + 1 = 0 (mod p)
 *      -> n = (p-1)/2 (mod p)
 *      -> n^2 = (p-1)^2 / 4 (mod p) [3]
 *  - From [1], we have that n^2 = p - k^2 (mod p) [4]
 *  - From [3] and [4], we have   (p-1)^2 / 4 = p - k^2 (mod p)
 *                           ->  p^2 - 2p + 1 = 4p - 4k^2 (mod p)
 *                           ->             1 =    - 4k^2 (mod p)
 *                           ->      4k^2 + 1 = 0 (mod p) [5]
 *  - From [5], we conclude that if p is the largest integer that satisfies [1] and [2],
 *    p must divide 4k^2 + 1. In other words, p must be the largest prime factor of 4k^2+1.
 *  - In this implementation, we'll rely on factorizing 4k^2+1 to find the largest prime factor p for each k.
 *  Runtime: ~10mins
 */

#include"pari/pari.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cassert>
#include<thread>
#include<sstream>

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
const ui MOD = pow(10, 18);

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

bool isprime(const ui& n)
{
    return ispseudoprime(stoi(n), 0);
}

ui largest_prime_factor(const ui& n)
{
    GEN factors = factor(stoi(n));
    // Docs: https://pari.math.u-bordeaux.fr/pub/pari/manuals/2.7.6/libpari.pdf
    // 4.2.3 Type recursivity -> gmael2 (getting an element from a t_MAT)
    // 5.2.3 Read type-dependent information -> lgcols, nbrows (gettings number of rows and columns of a t_MAT)
    // e.g., factors =
    // [    2 2]
    // [   13 1]
    // [19231 1]
    //cout << lgcols(factors) << endl; // = 4
    //cout << nbrows(factors) << endl; // = 3
    //cout << gtolong(gmael2(factors, 1, 1)) << endl; // = 2
    //cout << gtolong(gmael2(factors, 1, 2)) << endl; // = 13
    //cout << gtolong(gmael2(factors, 1, 3)) << endl; // = 19231
    //cout << gtolong(gmael2(factors, 2, 1)) << endl; // = 2
    //cout << gtolong(gmael2(factors, 2, 2)) << endl; // = 1
    //cout << gtolong(gmael2(factors, 2, 3)) << endl; // = 1
    return gtolong(gmael2(factors, 1, lgcols(factors)-1));
}

int main()
{
    pari_init(5000000000,2);
    ui ans = 0;
    vector<ui> small_primes = get_primes(10000);
    for (ui k = 1; k <= N; k++)
    {
        if (k % 10000 == 0)
            cout << k << " " << ans << endl;
        ui alpha = 4*k*k+1;
        ui largest_small_prime_factor = 1;
        bool no_large_prime_factor = false;
        for (auto p: small_primes)
        {
            if (alpha % p == 0)
                largest_small_prime_factor = p;
            while (alpha % p == 0)
                alpha /= p;
            if (alpha < p)
            {
                no_large_prime_factor = true;
                break;
            }
        }
        if (!no_large_prime_factor)
        {
            ui largest_remaining_prime_factor = largest_prime_factor(alpha);
            ans += max(largest_small_prime_factor, largest_remaining_prime_factor);
        }
        else
        {
            ans += largest_small_prime_factor;
        }
        ans %= MOD;
    }
    cout << ans << endl;
    pari_close();
    return 0;
}

