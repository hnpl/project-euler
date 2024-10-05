// Problem: https://projecteuler.net/problem=347

/*
    For each pair of primes (p, q),
        - find the largest k such that p^k * q < N.
        - for each m in [1..k], find the largest n such that p^m * q^n < N, keep the optimal result.
*/

#include<iostream>
#include<vector>
#include<unordered_map>
#include<cmath>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 10'000'000;
const ui SQRT_N = (ui)sqrt(N);

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
        if (is_prime[p])
            primes.push_back(2*p+3);
    return primes;
}

ui binary_search(const vector<ui>& vals, ui target)
{
    ui l = 0;
    ui r = vals.size()-1;

    ui m = 0;
    while (l < r)
    {
        if (vals[m] < target)
            l = m;
        else if (vals[m] > target)
            r = m + 1;
    }

    return m;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes(N);

    for (ui i = 0; i < primes.size(); i++)
    {
        for (ui j = i + 1; j < primes.size(); j++)
        {
            ui prime1 = primes[i];
            ui prime2 = primes[j];
            if (prime1 * prime2 > N)
                break;
            // prime1 < prime2
            ui l = prime1;
            ui r = prime2;
            while (l*prime1*r < N)
                l = l * prime1;
            ui optimal = l*r;
            while (r < N && l > 1)
            {
                r = r * prime2;
                while (l*r > N && l > 1)
                    l = l / prime1;
                if (l*r <= N && l > 1)
                    optimal = max(optimal, l*r);
            }
            ans = ans + optimal;
        }
    }

    cout << ans << endl;

    return 0;
}
