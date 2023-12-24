// Question: https://projecteuler.net/problem=772

// The result is https://oeis.org/A051426
// To compute lcm(2, 4, 6, ..., 2N), we can compute 2 * lcm(1, 2, 3, ..., N)
// lcm(1, 2, 3, ..., N) = product(P_1 ^ a_1, P_2 ^ a_2, ..., P_m ^ a_m)
//   where P_i are primes less than N
//   and a_i is the largest positive integer such that P_i ^ a_i <= N

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

typedef uint64_t ui;

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
}

const ui N = pow(10, 8);
const ui MOD = pow(10, 9) + 7;

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

ui pow_mod(const ui& base, ui exp, const ui& mod)
{
    ui ans = 1;
    ui w = base;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            ans *= w;
            ans %= mod;
        }
        w *= w;
        w %= mod;
        exp /= 2;
    };
    return ans;
}

int main()
{
    ui ans = 1;
    vector<ui> primes = get_primes(N);
    ui sqrt_N = ui(ceil(sqrt(N)));
    for (auto prime: primes)
    {
        if (prime > sqrt_N)
        {
            ans *= prime;
            ans %= MOD;
        }
        else
        {
            ui exp = ui(floor(log(N) / log(prime)));
            ans *= pow_mod(prime, exp, MOD);
            ans %= MOD;
        }
    }
    cout << (ans*2)%MOD << endl;
    return 0;
}
