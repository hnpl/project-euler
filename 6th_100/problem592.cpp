// Question: https://projecteuler.net/problem=592

/*
  - Strategy:
    . The main idea is that, we can calculate the product of this sequence (mod MOD) for some number A such that MOD = A^2,
        . The sequence, { (k*A+1), (k*A+3), (k*A+5), ..., (k*A+(2*n+1)) }
        . Why?
            (k*A+1) * (k*A+3) * (k*A+5) * ... * (k*A+(2*n+1)) [mod A^2]
          = something * A^(n+1) + something * A^(n) + ... + something * A^2 + (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * A + c [mod A^2]
            where c = 1 * 3 * 5 * ... * (2*n+1)
          =                   0 +                0  + ... +               0 + (c/1 + c/3 + c/5 + ... + c/(2*n+1)) k * A + c [mod A^2]
          = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * k * A + c
        . So, if the sequence is short enough, we can easily calculate the product (mod MOD) via bruteforce.
    . Let F(k, A, n) = product_([(k*A+1), (k*A+3), (k*A+5), ..., (k*A+(2*n+1))]) (mod MOD)
                     = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * k * A + c
                     where c = 1 * 3 * 5 * ... * (2*n+1)
    . Note that, by definition, the double factorial, e.g. (2*n+1)!!, is the product of odd numbers from 1 to 2*n+1.

    . So, the question asks for the last 12 digits of N! without the trailing zeros.
        . To get the last 12 digits of N!, we can do N! (mod 2^48)
        . However, to get the last 12 digits without the trailing zeros,
          we'd want to compute K * 2^(p (mod 4)) (mod 2^48) where N! = K * 2^p and p is such that N! is not divisible 2^(p+1) (i.e. p is the number of factors of 2 in N!)
            . We don't want to get rid all of factors of 2 in N! because 2^0 = 1, 2^1 = 2, 2^2 = 4, 2^3 = 8, and all of them does not produce 0 in the hexadecimal representation.
    . We can calculate p via,
        . p = sum(floor(N / 2^t)) for all t < log2(N).
    . So, how to calculate K?
        . Consider a small N,                                                                                                                                   The product of odd numbers in the sequence
                                                             20! = 1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13 * 14 * 15 * 16 * 17 * 18 * 19 * 20    | 19!!
removing 2^1 for even numbers that are not divisible by 2^2 |          1       .       3       .        5         .         7         .         9              | 9!!
removing 2^2 for even numbers that are not divisible by 2^3 |                  1               .                  3                   .                   5    | 5!!
removing 2^3 for even numbers that are not divisible by 2^4 |                                  1                                      .                        | 1!!
removing 2^4 for even numbers that are not divisible by 2^5 |                                                                         1                        | 1!!
        . Essentially, K = product ((N/2^t)!!) for all t < log2(N).
        . The next problem to efficiently calculate the double factorial (2*n+1)!! (mod MOD) for some large n.
            . Note that (2*n+1)!! is essentially the product of odd numbers upto 2*n+1.
            . We can divide the odd numbers sequence into multiple sequences, where each sequence is of size A such that A^2 = MOD (except for the tail sequence, which is of size B < A).
              For convenient, let's choose an even number A.
            . The product of the first sequence is      P_1 = (0*A+1) * (0*A+3) * (0*A+5) * ... * (0*A+A-1) = F(0, A, A/2) = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * 0 * A + c
              The product of the second sequence is     P_2 = (1*A+1) * (1*A+3) * (1*A+5) * ... * (1*A+A-1) = F(1, A, A/2) = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * 1 * A + c
              The product of the third sequence is      P_3 = (2*A+1) * (2*A+3) * (2*A+5) * ... * (2*A+A-1) = F(2, A, A/2) = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * 2 * A + c
              ...
              The product of the last full sequence is  P_m = (m*A+1) * (m*A+3) * (m*A+5) * ... * (m*A+A-1) = F(m, A, A/2) = (c/1 + c/3 + c/5 + ... + c/(2*n+1)) * m * A + c
              Let P_t be the product of the tail sequence. We can directly calculate P_t using product.
              for c = 1 * 3 * 5 * ... * (A-1) = (A-1)!!
            . In the code, we denote alpha as alpha = (c/1 + c/3 + c/5 + ... + c/(2*n+1))
            . So, (2*n+1)!! = P_1 * P_2 * ... * P_m * P_t (mod MOD)
                            = (alpha * 0 * A + c) * (alpha * 1 * A + c) * (alpha * 2 * A + c) * ... * (alpha * m * A + c) * P_t (mod MOD)
                            = (something * A^(m+1) + something * A^m + ... + something * A^2 + (c^m * alpha * 0 + c^m * alpha * 1 + ... + c^m * alpha * m) * A + c^(m+1)) * P_t (mod MOD)
                            = (                  0 +               0 + ... +               0 + c^m * alpha * A * sum(1..m) + c^(m+1)) * P_t (mod MOD)
                            = (c^m * alpha * A * m(m+1)/2 + c^(m+1)) * P_t (mod MOD)
            . Plug this back to K = product ((N/2^t)!!) for all t < log2(N).
*/

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
const ui cache_size = 1ULL << 24;

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

// n should be odd
ui calculate_double_factorial(ui n, const vector<ui>& factorial_cache, unordered_map<ui, ui>& sum_cache, const vector<ui>& inverse_mod_sum_cache)
{
    // assume that the cache size is an even number
    // we add one to n to make it easier to deal with
    n = n + 1;

    if (n <= cache_size)
        return factorial_cache[n-1];

    ui k = n / cache_size - 1;
    ui ans = 1;

    // full sum
    // k is only valid when n >= cache_size; e.g. n=20, cache_size=20 -> k=0 // full_sum up to k=0
    //                                            n=38, cache_size=20 -> k=0
    //                                            n=40, cache_size=20 -> k=1
    //                                            n=42, cache_size=20 -> k=1
    ans *= sum_cache[k];

    // tail calculation
    ui tail_n = n - (k+1)*cache_size;
    if (tail_n == 0)
        return ans;
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
        inverse_mod_sum_cache[n] = (inverse_mod_sum_cache[n-2] + inverse_mod(n, MOD)) % MOD;

    // warming up the sum_cache
    unordered_map<ui, ui> sum_cache;
    unordered_set<ui> interested_k;
    // -- deciding what sum to keep
    ui base = 2;
    while (base <= N)
    {
        ui k = (N-base/2)/base+1;
        k = 2*k;
        if (k == 0)
            continue;
        k = k/cache_size-1;
        interested_k.insert(k);
        base *= 2;
    }

    ui running_sum = 1;
    ui alpha = inverse_mod_sum_cache[cache_size-1]; // cache_size must be even
    ui c = factorial_cache[cache_size-1];
 
    for (auto k: interested_k)
    {
        if (k == 0)
        {
            sum_cache[k] = c;
            continue;
        }
        ui product = 1;
        product *= cache_size * alpha;
        product %= MOD;
        product *= pow_mod(c, k, MOD);
        product %= MOD;
        product *= k*(k+1)/2;
        product %= MOD;
        product += pow_mod(c, k+1, MOD);
        product %= MOD;
        sum_cache[k] = product;
    }

    base = 2;

    while (base <= N)
    {
        ui n = (N-base/2)/base+1;
        n = 2*n;
        ui double_factorial = calculate_double_factorial(n-1, factorial_cache, sum_cache, inverse_mod_sum_cache);
        ans *= double_factorial;
        ans %= MOD;
        base *= 2;
    }

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
