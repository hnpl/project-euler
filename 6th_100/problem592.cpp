// Question: https://projecteuler.net/problem=592

#include<pari/pari.h>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<iomanip>
#include<vector>
#include<unordered_map>
#include<unordered_set>
#include<cassert>

using namespace std;

typedef __uint128_t ui;
#define endl "\n"

constexpr ui factorial(ui n)
{
    if (n == 0)
        return 1;
    return factorial(n-1) * n;
};

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = factorial(20);
const ui M = 48;
const ui MOD = 1ULL << M;
const ui cache_size = 1ULL << 29;

//const ui N = 20;
//const ui cache_size = 1 << 2;
//const ui MOD = 1 << 4;

string uint128_to_string(ui n, ui base)
{
    stringstream stream;

    if (n == 0)
        stream << "0";

    vector<char> chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

    while (n > 0)
    {
        char r = n % base;
        stream << chars[r];
        n = n / base;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    return str;
}

ostream& operator<<(ostream& os, const __uint128_t& n)
{
    os << uint128_to_string(n, 16);
    return os;
}

ui inverse_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

// n should be odd
ui calculate_double_factorial(ui n, const vector<ui>& factorial_cache, unordered_map<ui, ui>& sum_cache, const vector<ui>& inverse_mod_sum_cache)
{
    //cout << "calculating " << n <<"!!" << endl;
    // assume that the cache size is an even number
    // we add one to n to make it easier to deal with
    n = n + 1;

    if (n <= cache_size)
        return factorial_cache[n-1];

    ui k = n / cache_size - 1;
    ui ans = 1;
    //cout << "+-- k: " << k << endl;

    // full sum
    // k is only valid when n >= cache_size; e.g. n=20, cache_size=20 -> k=0 // full_sum up to k=0
    //                                            n=39, cache_size=20 -> k=0
    //                                            n=40, cache_size=20 -> k=1
    //                                            n=42, cache_size=20 -> k=1
    ans *= sum_cache[k];
    //cout << "+-- sum_cache[k]: " << sum_cache[k] << endl;
    //if (n<(k+1)*cache_size)
    //    return ans;

    // tail calculation
    ui tail_n = n - (k+1)*cache_size;
    //cout << "+-- tail_n: " << tail_n << endl;
    if (tail_n == 0)
        return ans;
    //cout << " ** tail " << tail_n << " " << factorial_cache[tail_n] << " " << inverse_mod_sum_cache[tail_n] << endl;
    ui tail_c = factorial_cache[tail_n-1];
    ui tail_alpha = inverse_mod_sum_cache[tail_n-1];
    ans *= (((((((k+1) * cache_size) % MOD) * tail_c) % MOD) * tail_alpha) % MOD + tail_c);
    ans %= MOD;

    return ans;
}

int main()
{
    ui ans = 1;

    // warming up the factorial_cache up to cache_size
    vector<ui> factorial_cache = vector<ui>(cache_size+1, 0);
    factorial_cache[1] = 1;
    for (ui n = 3; n <= cache_size; n+=2)
        factorial_cache[n] = (factorial_cache[n-2] * n) % MOD;

    // warming up the inverse_mod_sum_cache up to cache_size
    vector<ui> inverse_mod_sum_cache = vector<ui>(cache_size+1, 0);
    inverse_mod_sum_cache[1] = inverse_mod(1, MOD);
    for (ui n = 3; n <= cache_size; n+=2) // this loop dominates the warming up phase
    {
        //if (n % 1000001 == 0)
        //    cout << n << endl;
        inverse_mod_sum_cache[n] = (inverse_mod_sum_cache[n-2] + inverse_mod(n, MOD)) % MOD;
    }

    // warming up the sum_cache
    unordered_map<ui, ui> sum_cache;
    unordered_set<ui> interested_k;
    // -- deciding what sum to keep
    ui base = 2;
    while (base <= N)
    {
        ui k = (N-base/2)/base+1;
        k = 2*k;
        k = k/cache_size-1;
        interested_k.insert(k);
        base *= 2;
    }

    ui running_sum = 1;
    ui alpha = inverse_mod_sum_cache[cache_size-1]; // cache_size must be even
    ui c = factorial_cache[cache_size-1];
   
    for (ui k = 0; k <= N / cache_size; k++)
    {
        //if (k % 1000001 == 0)
        //    cout << k << endl;
        running_sum *= ((((((k * cache_size) % MOD) * c) % MOD) * alpha) % MOD) + c;
        running_sum %= MOD;
        if (interested_k.find(k) != interested_k.end())
        {
            sum_cache[k] = running_sum;
            //cout << "sum_cache[" << k << "]=" << sum_cache[k] << endl;
        }
    }

    base = 2;

    while (base <= N)
    {
        ui n = (N-base/2)/base+1;
        n = 2*n;
        ui double_factorial = calculate_double_factorial(n-1, factorial_cache, sum_cache, inverse_mod_sum_cache);
        ans *= double_factorial;
        ans %= MOD;
        //cout << ">>>>>>" << " " << double_factorial << endl;
        base *= 2;
    }
    cout << ans << endl;

    // power of 2 counting
    ui count_power_of_2 = 0;
    base = 2;
    while (base <= N)
    {
        count_power_of_2 += N / base;
        base *= 2;
    }
    count_power_of_2 %= 4;

    ans = ans * (pow(2, count_power_of_2));
    ans %= MOD;

    cout << "ans: " << ans << endl;

    return 0;
}
