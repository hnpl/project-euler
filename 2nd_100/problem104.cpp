// Problem: https://projecteuler.net/problem=104

/*
    - Keep track of the last k digits using (mod 10**k).
    - Approximate the first k digits by keeping track of a lot more than k digits (avoiding precision issues) and doing normal adding operation.
*/

#include<iostream>
#include<vector>
#include<array>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui MOD = 1'000'000'000ULL;
const ui M = 18;

constexpr ui pow10(ui exponent)
{
    if (exponent == 0)
        return 1;
    return 10 * pow10(exponent-1);
}


bool is_pandigital(ui n)
{
    static vector<ui> freq = vector<ui>(10);
    fill(freq.begin(), freq.end(), 0);
    ui sum_freq = 0;

    while (n > 0)
    {
        ui digit = n % 10;
        if (freq[digit] == 1)
            return false;
        freq[digit] += 1;
        sum_freq += 1;
        n = n / 10;
    }

    if (freq[0] == 0 && sum_freq == 9)
        return true;
    return false;
}

ui get_first_k_digits(ui n, ui k)
{
    while (n > pow10(k))
        n = n / 10;
    return n;
}

int main()
{
    ui a = 1;
    ui b = 1;

    ui a_first_digits = 1;
    ui b_first_digits = 1;

    ui k = 2;

    while (true)
    {
        k = k + 1;

        ui t = a;
        a = b;
        b = t + b;
        b = b % MOD;

        t = a_first_digits;
        a_first_digits = b_first_digits;
        b_first_digits = t + b_first_digits;
        while (a_first_digits > pow10(M))
        {
            a_first_digits = a_first_digits / 10;
            b_first_digits = b_first_digits / 10;
        }
        if (is_pandigital(b) && is_pandigital(get_first_k_digits(b_first_digits, 9)))
            break;
    }

    cout << k << endl;

    return 0;
}

