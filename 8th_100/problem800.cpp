// Question: https://projecteuler.net/problem=800

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>

using namespace std;

typedef uint64_t ui;

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
    const double upperbound = log(800800) * 800800;
    auto primes = get_primes(20000000);
    ui n = primes.size();
    ui count = 0;
    for (ui i = 0; i < n; i++)
    {
        ui l = 0;
        ui r = n - 1;
        ui p = primes[i];
        while (l < r)
        {
            ui m = (l+r+1)/2;
            ui q = primes[m];
            double ll = log(p) * q + log(q) * p;
            if (ll <= upperbound)
                l = m;
            else
                r = m - 1;
        }
        if (l < i)
            break;
        ui q = primes[l];
        count += l - i;
        //for (ui j = i+1; j < n; j++)
        //{
        //    ui p = primes[i];
        //    ui q = primes[j];
        //    double l = log(p) * q + log(q) * p;
        //    if (l > upperbound)
        //        break;
        //    count += 1;
        //}
    }
    cout << count << endl;
}
