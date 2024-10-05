// Problem: https://projecteuler.net/problem=211

#include<iostream>
#include<vector>
#include<queue>
#include<cmath>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 64000000;
const ui SQRT_N = (ui)sqrt(N);

inline bool is_square(ui n)
{
    ui s = sqrt(n);
    if ((s*s == n) || ((s+1)*(s+1) == n) || ((s-1)*(s-1) == n))
        return true;
    return false;
};

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

int main()
{
    ui ans = 0;

    vector<ui> sigma2 = vector<ui>(N, 1);

    // For each integer k, add the square of divisor 1 and the square of divisor k
    for (ui i = 1; i < N; i++)
        sigma2[i] = 1 + i*i;
    sigma2[1] = 1;

    vector<ui> primes = get_primes(SQRT_N);
    
    for (ui divisor = 2; divisor <= SQRT_N; divisor++)
    {
        for (ui n = 2*divisor; n <= N; n+=divisor)
        {
            ui idiv = n / divisor;
            if (idiv < divisor) // avoid the case where we already counted the pair (divisor, idiv)
                continue;
            sigma2[n] += divisor * divisor;
            if (idiv != divisor) // avoid the case where n is a square number
                sigma2[n] += idiv * idiv;
            
        }
    }

    /*
    vector<ui> primes = get_primes(N);

    vector<ui> exponents = vector<ui>(N);
    vector<bool> filled = vector<bool>(N, false);
    for (auto prime: primes)
    {
        fill(exponents.begin(), exponents.end(), 1);
        cout << prime << endl;
        sigma2[prime] = 1 + prime * prime;
        filled[prime] = true;
        for (auto factor = prime*prime; factor < N; factor *= prime)
        {
            sigma2[factor] = sigma2[factor/prime] + factor*factor;
            filled[factor] = true;
        }
        for (auto factor = prime; factor < N; factor *= prime)   
        {
            for (auto i = factor; i < N; i += factor)
                exponents[i] *= prime;
        }
        for (auto i = prime; i < N; i+=prime)
            if (!filled[i])
                sigma2[i] *= sigma2[exponents[i]];
            
    }
    */


    
    for (auto it = sigma2.begin(); it != sigma2.end(); it++)
        if (is_square(*it))
            ans = ans + it - sigma2.begin();
        
    cout << ans << endl;

    return 0;
}
