#include<pari/pari.h>
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<cassert>
#include<sstream>
#include<cmath>

using namespace std;

typedef __int128_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return base * exp;
};

const ui N = pow(10, 6);
const ui K = pow(10, 12);
const ui MOD = pow(10, 9) + 7;

ui pow_mod(const ui& base, const ui& exp, const ui& mod)
{
    ui b = base;
    ui e = exp;
    ui ans = 1;
    while (e > 0)
    {
        if (e % 2 == 1)
        {
            ans *= b;
            ans %= mod;
        }
        e /= 2;
        b *= b;
        b %= mod;
    }
    return ans;
}

string i128_to_string(__int128_t n)
{
    stringstream stream;
    if (n < 0)
        n+=MOD;
    bool sign = false;
    if (n < 0)
    {
        sign = true;
        n = -n;
    }
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }
    if (sign)
        stream << "-";

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __int128_t& n)
{
    os << i128_to_string(n);
    return os;
}

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

ui inv_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

string concat_nums(const vector<ui>& nums)
{
    stringstream s;
    for (auto n: nums)
        s << n;
    return s.str();
}

ui MVC(const ui& a, const ui& b, const ui& mod) // multiply very carefully
{
    return (a * b) % mod;
}

ui S(const string& pattern, const ui& n_periods)
{
    ui ans = 0;
    ui period_length = pattern.size();
    ui n_digits = MVC(n_periods, period_length, MOD);

    ui L = period_length;
    ui N = n_periods;

    ui pow_10_L = pow_mod(10, L, MOD);
    ui pow_10_LN = pow_mod(10, L*N, MOD);
    ui pow_10_L1_2 = pow_mod((pow_10_L-1) % MOD, 2, MOD);
    ui pow_N_2 = pow_mod(N, 2, MOD);

    ui inv_mod_9 = inv_mod(9, MOD);

    for (ui inversed_place_value = 0; inversed_place_value < period_length; inversed_place_value++)
    {
        if (inversed_place_value % 100000 == 0)
            cout << inversed_place_value << endl;

        ui digit = pattern[inversed_place_value] - '0';
        ui offset = inversed_place_value + 1;
        ui a = offset;

        ui pow_10_La1 = pow_mod(10, L+1-a, MOD);

        ui part_1_a = MVC(pow_10_La1, (pow_10_LN-1)%MOD, MOD);
        ui part_1_b = (pow_10_L-1)%MOD;
        ui inv_mod_part_1_b = inv_mod(part_1_b, MOD);
        ui part_1 = (MVC(part_1_a, inv_mod_part_1_b, MOD) - N) % MOD;

        ui part_2_a = MVC(N, pow_10_L-1, MOD);
        part_2_a = MVC(part_2_a, pow_10_L - MVC(2,pow_10_La1,MOD) - 1, MOD);
        ui part_2_b = MVC(2, MVC(pow_10_La1, (pow_10_LN - 1) % MOD, MOD), MOD);
        ui part_2_c = MVC(pow_N_2, pow_10_L1_2, MOD);
        ui part_2_d = MVC(2, pow_10_L1_2, MOD);
        ui inv_mod_part_2_d = inv_mod(part_2_d, MOD);
        ui part_2 = MVC((((part_2_a + part_2_b) % MOD) - part_2_c) % MOD, inv_mod_part_2_d, MOD);
        //cout << part_2 << endl;

        ui coeff = MVC(a, part_1, MOD) + MVC(L, part_2, MOD);
        coeff %= MOD;
        ans += MVC(MVC(coeff, digit, MOD), inv_mod_9, MOD);
        ans %= MOD;
    }
    return ans;
}

int main()
{
    auto primes = get_primes(15485864);
    auto C = concat_nums(primes);
    ui ans = S(C, K);
    if (ans < 0)
        ans += MOD;
    cout << ans << endl;
    return 0;
}
