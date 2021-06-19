// Question: https://projecteuler.net/problem=757
//
// We have ab=cd=N and a+b=c+d+1 (Note that N is divisible by a)
// So, b=N/a and d=N/c, which means a + N/a = d + N/d + 1
//                      which means N = ad(1+d-a)/(d-a)
// WLOG, let p = d-a where p is a positive integer, we have N = a(a+p)(1+p)/p = (a^2)/p + (a^2) + a + a*p.
// Note that since p > 0, we have d > a.
// This means a^2 must be divisible by p (*)
// Since N = (a^2)/p + (a^2) + a + a*p, which means (a^2)/p must be divisible by a (**)
// From (*) and (**), we have that `a` must be divisible by p.
// Also, since N = ad(1+d-a)/(d-a) = ad/(d-a) + ad > ad > a^2, we can iterate through all `a` such that (a^2)< N,
// find all possible p (which is a divisor of `a`) to find N.

#include <iostream>
#include <unordered_set>
#include <cassert>
#include <cmath>

typedef uint64_t ui;

#define endl "\n"

constexpr ui pow10(ui exp)
{
    if (exp == 0)
        return 1;
    return 10 * pow10(exp-1);
};

const ui N = pow10(14);

using namespace std;

int main()
{
    unordered_set<ui> ans;

    ui a = 1;
    ui aa = a * a;

    while (aa <= N)
    {
        ui sqrt_a = ui(sqrt(double(a)));
        for (ui divisor = 1; divisor <= sqrt_a; divisor++)
        {
            if (a % divisor == 0)
            {
                ui p = divisor;
                ui n = aa / p + aa + a + a*p;
                if (n <= N)
                    ans.insert(n);
                p = a / divisor;
                n = aa / p + aa + a + a*p;
                if (n <= N)
                    ans.insert(n);
            }
        }
        a += 1;
        aa = a * a;
    }

    cout << ans.size() << endl;
    return 0;
}
