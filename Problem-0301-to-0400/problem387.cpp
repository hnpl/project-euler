// Problem: https://projecteuler.net/problem=387
// g++ problem387.cpp -O3 -lpari -L$HOME/pari/lib -I$HOME/pari/include

/*
    Find the sum of prime number p such that p[:-1] is a Harshad number and p[:-1] / sum_digits(p[:-1]) is a prime.
*/

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<unordered_map>
#include<numeric>
#include<algorithm>
#include<cmath>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 100'000'000'000'000ULL;
//const ui N = 10000ULL;
const ui N_DIGITS = ui(log10(N));

// return 0 if n is not a Harshad number
//        1 if n is a strong Harshad number
//        2 if n is a Harshad number but not strong
ui is_Harshad_number(ui n)
{
    ui digit_sum = 0;
    ui m = n;
    while (m > 0)
    {
        digit_sum = digit_sum + m % 10;
        m = m / 10;
    }
    if ((n % digit_sum) == 0)
    {
        if (uisprime(n/digit_sum))
            return 1;
        return 2;
    }
    return 0;
}

ui sum_strong_right_truncatable_Harshad_primes(ui strong_Harshad_number)
{
    ui sum = 0;
    for (ui digit = 1; digit <= 9; digit+=2)
    {
        ui n = strong_Harshad_number*10 + digit;
        if (uisprime(n))
            sum = sum + n;
    }
    return sum;
}

int main()
{
    ui ans = 0;

    pari_init(100000000, 2);

    vector<ui> newly_added;

    for (ui i = 1; i <= 9; i++)
        newly_added.push_back(i);

    for (ui n_digits = 2; n_digits <= N_DIGITS-1; n_digits++)
    {
        vector<ui> old_added = newly_added;
        newly_added = vector<ui>();
        for (auto p: old_added)
        {
            for (ui digit = 0; digit <= 9; digit++)
            {
                ui n = p * 10 + digit;
                ui type = is_Harshad_number(n);
                if (type >= 1)
                {
                    newly_added.push_back(n);
                    if (type == 1)
                        ans = ans + sum_strong_right_truncatable_Harshad_primes(n);
                }
            }
        }
    }

    

    cout << ans << endl;

    pari_close();

    return 0;
}

