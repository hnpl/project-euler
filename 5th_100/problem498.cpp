// Question: https://projecteuler.net/problem=772
//
// . We have,
//   x^n = (1 + x - 1)^n
//       = C(n,0)*(x-1)^0 + C(n,1)*(x-1)^1 + C(n,2)*(x-1)^2 + C(n,3)*(x-1)^3 + ... + C(n,n)*(x-1)^n
//   So, f(x) = (x^n) % ((x-1)^m) = C(n,0)*(x-1)^0 + C(n,1)*(x-1)^1 + ... + C(n,m-1)*(x-1)^(m-1)
//   So, the coefficient of x^d in f(x) is,
//      g(n,m,d) = abs(C(n,d)*C(d,0) - C(n,d+1)*C(d+1,1) + C(n,d+2)*C(d+2,2) - ... + (-1)**(m-d-1) * C(n,m-1)*C(m-1,m-d-1))
//               = abs(sum[(-1)**k * (C(n,k+d)*C(k+d,k))]) for k in [0, m-d-1]
//   Note that, C(n,k+d)*C(k+d,k) = n! / (k+d)! / (n-k-d)! * (k+d)! / d! / k!
//                                = n! / (n-k-d)! / d! / k!
//                                = n! / d! / (n-d)! * (n-d)! / k! / (n-d-k)!
//                                = C(n,d) * C(n-d, k)
//   So, g(n,m,d) = abs(C(n,d) * sum[(-1)**k * C(n-d,k)]) for k in [0, m-d-1]
//   Also, note that, C(n-d,k) = C(n-d-1, k) + C(n-d-1, k-1)
//       C(n-d, 0) - C(n-d, 1) + C(n-d, 2) - C(n-d, 3) + C(n-d, 4) - ... + (-1)**(m-d-1) * C(n-d,k)
//     =         1 - C(n-d-1, 1) - C(n-d-1, 0) + C(n-d-1, 2) + C(n-d-1, 1) - C(n-d-1, 3) - C(n-d-1, 2) + C(n-d-1, 4) + C(n-d-1, 3) + ...
//     = (-1)**(m-d-1) * C(n-d-1, m-d-1)
//   So, g(n,m,d) = abs(C(n,d) * (-1)**(m-d-1) * C(n-d-1, m-d-1))
//                = C(n,d) * C(n-d-1, m-d-1)
//
// . We want to compute ans = C(n,d) * C(n-d-1, m-d-1) mod p
//     . Note that the term "n" and the term "n-d-1" and the term "m-d-1" are large.
//     . Also note that, C(n, k) = n! / k! / (n-k)!
//     . Also note that, n!/p^k mod p = [(p! mod p) ** (n/p)] * [(n%p)! mod p] * [(n/p)! mod p] where n! is divisible by p^k and not p^(k+1); i.e., we are computing the factorial n! mod p but getting rid of all p factor in n!.

#include<pari/pari.h>
#include<iostream>
#include<vector>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = pow(10, 13);
const ui M = pow(10, 12);
const ui D = pow(10, 4);
//const ui N = 100;
//const ui M = 10;
//const ui D = 4;
const ui MOD = 999999937;
//const ui MOD = 53;

ui inverse_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

ui pow_mod(ui base, ui exp, const ui& mod)
{
    ui ans = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            ans *= base;
            ans %= mod;
        }
        base *= base;
        base %= mod;
        exp = exp / 2;
    }

    return ans;
}

ui binomial_mod(const ui& n, const ui& k, const ui& mod, const vector<ui>& cached_factorial_mod, const vector<ui>& cached_inversed_factorial_mod)
{
    ui ans = 1;

    ui one_factorial_period = 1;
    for (ui i = 1; i < mod; i++)
    {
        one_factorial_period *= i;
        one_factorial_period %= mod;
    }

    ui n_periods = n / mod;
    ui n_remaining = n % mod;

    ans *= pow_mod(one_factorial_period, n_periods, mod);
    ans %= mod;
    ans *= cached_factorial_mod[n_remaining];
    ans %= mod;
    ans *= cached_factorial_mod[n_periods];
    ans %= mod;

    ui k_periods = k / mod;
    ui k_remaining = k % mod;

    ans *= pow_mod(inverse_mod(one_factorial_period, mod), k_periods, mod);
    ans %= mod;
    ans *= cached_inversed_factorial_mod[k_remaining];
    ans %= mod;
    ans *= cached_inversed_factorial_mod[k_periods];
    ans %= mod;

    ui n_k_periods = (n-k) / mod;
    ui n_k_remaining = (n-k) % mod;

    ans *= pow_mod(inverse_mod(one_factorial_period, mod), n_k_periods, mod);
    ans %= mod;
    ans *= cached_inversed_factorial_mod[n_k_remaining];
    ans %= mod;
    ans *= cached_inversed_factorial_mod[n_k_periods];
    ans %= mod;

    return ans;
}

ui C(const ui& n, const ui& m, const ui& d, const ui& mod)
{
    ui ans = 1;

    vector<ui> cached_factorial_mod = vector<ui>(mod, 0);
    vector<ui> cached_inversed_factorial_mod = vector<ui>(mod, 0);
    cached_factorial_mod[0] = 1;
    cached_inversed_factorial_mod[0] = 1;
    for (ui k = 1; k < mod; k++)
        cached_factorial_mod[k] = (cached_factorial_mod[k-1] * k) % mod;
    for (ui k = 1; k < mod; k++)
        cached_inversed_factorial_mod[k] = (cached_inversed_factorial_mod[k-1] * inverse_mod(k, mod)) % mod;

    ans *= binomial_mod(n, d, mod, cached_factorial_mod, cached_inversed_factorial_mod);
    ans %= mod;

    cout << ans << endl;

    ans *= binomial_mod(n-d-1, m-d-1, mod, cached_factorial_mod, cached_inversed_factorial_mod);
    ans %= mod;

    return ans;
}

int main()
{
    ui ans = C(N, M, D, MOD);
    cout << ans << endl;
    return 0;
}
