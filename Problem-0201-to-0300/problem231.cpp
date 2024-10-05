// Question:https://projecteuler.net/problem=231

#include<iostream>
#include<vector>
#include<cmath>
#include<cassert>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 20000000;

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


int main()
{   
    ui ans = 0;

    vector<ui> primes = get_primes(N);

/*
    vector<ui> power_20m = vector<ui>(N+1, 0);
    vector<ui> power_15m = vector<ui>(N+1, 0);
    vector<ui> power_5m = vector<ui>(N+1, 0);
*/

    vector<ui> power_20m = vector<ui>(N+1, 0);
    vector<ui> power_15m = vector<ui>(15000000, 0);
    vector<ui> power_5m = vector<ui>(5000000, 0);
    for (auto prime: primes)
    {
        vector<ui> exp;
        ui p = prime;
        while (p <= N)
        {
            exp.push_back(p);
            p = p * prime;
        }
        // the number of p^k divisors of n! is : floor(n / p^k)
        // the number of p^i divisors (for all i) of n! is: sum_i (floor(n/p^i))
        // ^^^^^
        // Legendre's formula / de Polignac's formula
        for (auto e: exp)
        {
            power_20m[prime] = power_20m[prime] + 20000000 / e;
            power_15m[prime] = power_15m[prime] + 15000000 / e;
            power_5m[prime] = power_5m[prime] + 5000000 / e;
        }
    }

    /*
    for (auto prime: primes)
        ans = ans + prime * (power_20m[prime] - power_15m[prime] - power_5m[prime]);
    */

    for (auto prime: primes)
        ans = ans + prime * power_20m[prime];
    for (auto prime: primes)
    {
        if (prime > 15000000)
            break;
        ans = ans - prime * power_15m[prime];
    }
    for (auto prime: primes)
    {
        if (prime > 5000000)
            break;
        ans = ans - prime * power_5m[prime];
    }
    cout << ans << endl;

    return 0;
}