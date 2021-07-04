// https://projecteuler.net/problem=182

// g++ problem182.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -Wl,-rpath $HOME/pari/lib -lpari -O3

/*
 * We want to find all e such that the number m such that m^e=m(mod n).
 *
 * Since for a base m, there is a number p such that m^(1+k*p) == m^(1+(k+1)*p). Let p be the cycle length.
 * We can instead, for all message m, we can find the cycle length of m^e(mod n). From the cycle, we can 
 * find all e such that m^e = m^1(mod n).
 */

#include <pari/pari.h>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include <unordered_set>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui P = 1009;
const ui Q = 3643;
const ui N = P*Q;
const ui PHI = (P-1)*(Q-1);
const vector<ui> PHI_prime_factors = {2, 3, 7, 607};

bool is_coprime_with_phi(ui n)
{
    for (const auto &prime_factor: PHI_prime_factors)
        if (n % prime_factor == 0)
            return false;
    return true;
}

ui find_exp_cycle_length(ui base) // assume mod = 1009 * 3643
{
    ui length = 0;

    if (!((base % 1009 == 0) || (base % 3643 == 0)))
        return gtolong(order(gmodulss(base, N)));

    unordered_set<ui> seen;

    ui exp = 2;
    ui a = (base * base) % N;

    while ((exp <= N) || (seen.find(a) == seen.end()))
    {
        seen.insert(a);
        exp += 1;
    }
    length = exp - 1;

    return length;
}

int main()
{
    ui ans = 0;

    vector<ui> e_count = vector<ui>(PHI+1, 0);
    pari_init(500000000,2);

    // for every e, when m = 0 or m = 1, m^e=m(mod n)
    for (ui m = 2; m <= N-1; m++)
    {
        if ((m == 1009) || (m == 1009*1009) || (m == 3643))
            continue;
        ui cycle_length = find_exp_cycle_length(m);
        for (ui e = 1; e <= PHI; e += cycle_length)
            e_count[e] += 1;
        if (m % 100000 == 0)
            cout << m << endl;
    }

    ui curr_min = -1ULL;

    for (ui e = 2; e < PHI; e++)
    {
        if (is_coprime_with_phi(e))
        {
            if (e_count[e] < curr_min)
            {
                ans = e;
                curr_min = e_count[e];
            }
            else if (e_count[e] == curr_min)
            {
                ans += e;
            }
        }
    }


    cout << ans << endl;
    pari_close();

    return 0;
}
