// Question: https://projecteuler.net/problem=657

#include"pari/pari.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<sstream>

using namespace std;

typedef __int128_t si; // integer overflow occured for int64_t

constexpr si pow(si base, si exp)
{
    if (exp == 0)
        return 1;
    return pow(base, exp-1) * base;
};

const si ALPHA = pow(10, 7);
const si L = pow(10, 12);
const si MOD = pow(10, 9) + 7;

string int128_to_string(__int128_t n)
{
    stringstream stream;
    bool neg = n < 0;
    if (neg)
        n = -n;
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }
    if (neg)
        stream << "-";

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __int128_t& n)
{
    os << int128_to_string(n);
    return os;
}

si inv_mod(const si& n, const si& mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

void pre_compute_C(vector<si>& C, const si& n, const si& mod)
{
    C[0] = C[n] = 1;
    for (si k = 1; k <= n/2; k++)
    {
        si next_C = C[k-1];
        next_C *= (n-k+1);
        next_C %= mod;
        next_C *= inv_mod(k, mod);
        next_C %= mod;
        C[k] = C[n-k] = next_C;
    }
}

si pow_mod(si base, si exp, const si& mod)
{
    si ans = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            ans *= base;
            ans %= mod;
        }
        base *= base;
        base %= mod;
        exp /= 2;
    }

    return ans;
}

int main()
{
    si ans = 0;

    vector<si> C(ALPHA+1, 0);
    pre_compute_C(C, ALPHA, MOD);

    si coeff = 1;
    for (si j = ALPHA-1; j > 0; j--)
    {
        si new_term = 1;
        if (j > 1)
        {
            new_term *= C[j];
            new_term %= MOD;
            new_term *= (pow_mod(j, L+1, MOD) - j);
            new_term %= MOD;
            new_term *= inv_mod(j-1, MOD);
            new_term %= MOD;
        }
        else
        {
            new_term = C[j] * L;
            new_term %= MOD;
        }

        ans += coeff * new_term;
        ans %= MOD;
        coeff = -coeff;
    }
    cout << (ans + 1 + MOD) % MOD << endl;

    return 0;
}
