// Problem: https://projecteuler.net/problem=134

//g++ problem134.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -lpari -O3

/*
    For all pairs of consecutive primes (p1, p2),
        . Use Chinese Remainder Theorem to find v such that v = p1 (mod 10^K) and v = 0 (mod p2); where K is the number of digits of p1.
*/

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 1000000;

constexpr ui p10(ui n)
{
    if (n == 0)
        return 1;
    return 10 * p10(n-1);
};

const vector<ui> pow10 = {p10(0), p10(1), p10(2), p10(3), p10(4), p10(5), p10(6), p10(7)};

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

ui n_digits(ui n)
{
    ui ans = 0;

    while (pow10[ans] < n)
        ans = ans + 1;

    return ans;
}

ui find_prime_pair_connection(ui p1, ui p2)
{
    ui n_digits_p1 = n_digits(p1);

    GEN crt = chinese(gmodulss(p1, pow10[n_digits_p1]), gmodulss(0, p2));
    ui ans = gtolong(gel(crt, 2));

    return ans;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes(N+100);
    pari_init(50000000,2);

    ui n = primes.size();

    for (ui i = 2; i < n - 1; i++)
    {
        if (primes[i] > N)
            break;
        ans = ans + find_prime_pair_connection(primes[i], primes[i+1]);
    }

    cout << ans << endl;

    pari_close();

    return 0;
}
