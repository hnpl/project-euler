// Question: https://projecteuler.net/problem=357

// p = n + 1
// p'  = n/2 + 2              // n is even -> 2|n -> n/2 + 2 is prime
//     = (p - 1)/2 + 2
// 2p' = p - 1 + 4 = p + 3
// 2p' - 3 = p

// if 3|n: n/3 + 3 = p' -> n = 3p' - 9

#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 100000000;

int main()
{
    ui ans = 0;

    ui sqrt_N = ui(sqrt(N));

    vector<bool> is_prime = vector<bool>(N+1, true);
    is_prime[0] = is_prime[1] = false;
    for (ui i = 2; i <= sqrt_N; i++)
    {
        if (!is_prime[i])
            continue;
        for (ui j = 2*i; j <= N; j+=i)
            is_prime[j] = false;
    }

    vector<ui> candidates;
    candidates.push_back(1);
    for (ui pp = 3; 2*pp - 4 <= N; pp+=2)
    {
        ui n = 2*pp - 4;
        if (is_prime[pp] && is_prime[n+1])
        {
            if (!(n%3==0) || (is_prime[n/3+3]))
            candidates.push_back(n);
        }
    }

    for (auto n: candidates)
    {
        ui sqrt_n = ui(sqrt(n));
        bool valid = true;
        for (ui i = 2; i <= sqrt_n; i++)
            if (n % i == 0)
                if (!is_prime[n/i + i])
                {
                    valid = false;
                    break;
                }
        if (valid)
            ans = ans + n;
    }
    cout << ans << endl;

    return 0;
}
