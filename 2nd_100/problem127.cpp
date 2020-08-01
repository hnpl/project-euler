// Problem: https://projecteuler.net/problem=127

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<numeric>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 120000;

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

vector<ui> get_rads(ui n, const vector<ui>& primes)
{
    vector<ui> rads = vector<ui>(n+1, 1);

    for (auto prime: primes)
        for (ui i = prime; i <= n; i+=prime)
            rads[i] = rads[i] * prime;

    return rads;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes(N);
    vector<ui> rads = get_rads(N, primes);
    vector<ui> c_over_rads = rads;

    for (ui c = 1; c < c_over_rads.size(); c++)
        c_over_rads[c] = c / c_over_rads[c];

    // find abc-hits
    for (ui a = 1; a < N; a++)
    {
        for (ui b = a + 1; a + b < N; b++)
        {
            ui c = a + b;
            
            // rads[abc] = rads[a] * rads[b] * rads[c]
            // rads[a] * rads[b] * rads[c] < c , so rads[a] * rads[b] < c / rads[c]

            // if ((gcd(a,b) == 1) && (rads[a] * rads[b] < c_over_rads[c]))     // 10s slower
            // if ((rads[a] * rads[b] < c/rads[c]) && (gcd(a, b) == 1))         // 10x slower
            if ((rads[a] * rads[b] < c_over_rads[c]) && (gcd(a, b) == 1))
                ans = ans + c;
        }
    }

    cout << ans << endl;

    return 0;
}
