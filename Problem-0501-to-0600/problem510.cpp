// Question: https://projecteuler.net/problem=510

/*
 * . Due to https://en.wikipedia.org/wiki/Descartes%27_theorem#Special_cases, we have that,
        1/c = 1/a + 1/b + 2/sqrt(ab)
     -> c = (a+b+2sqrt(ab))/(ab)
          = ab / (sqrt(a) + sqrt(b))**2
   . Note that, a and b are not necessarily square numbers. However, if they are not square number,
   we still have the constraint that a*b is a square number (due to sqrt(ab)).
   . Also, if (a,b,c) is a solution, then (ka, kb, kc) is also a solution for all positive integers k.
     Assume that a <= b, then we have k <= N / b.
   . So, if we have a triplet (a,b,c) such that gcd(a,b,c) = 1, then we can discover all k.
 */

#include<cmath>
#include<iostream>
#include<numeric>
#include<utility>
#include<map>
#include<unordered_map>
#include<numeric>

using namespace std;

typedef uint64_t ui;

const ui N = 1000000000;
const ui SQRT_N = sqrt(N);

ui solve()
{
    ui ans = 0;
    for (ui sqrt_a = 1; sqrt_a <= SQRT_N; sqrt_a++)
    {
        ui a = sqrt_a * sqrt_a;
        for (ui sqrt_b = sqrt_a; sqrt_b <= SQRT_N; sqrt_b++)
        {
            ui b = sqrt_b * sqrt_b;
            ui _n = a*b;
            ui _d = (sqrt_a + sqrt_b) * (sqrt_a + sqrt_b);
            if (_n % _d == 0)
            {
                ui c = _n / _d;
                if (!(gcd(sqrt_a,gcd(sqrt_b,c)) == 1))
                    continue;
                ui k_max = N / b;
                ans += k_max * (k_max + 1) / 2 * (a + b + c);
            }

        }
        if (sqrt_a % 100 == 0)
            cout << sqrt_a << endl;
    }
    return ans;
}

ui brute_force()
{
    map<ui, ui> s;
    ui ans = 0;
    for (ui a = 1; a <= N; a++)
    {
        for (ui b = a; b <= N; b++)
        {
            ui sqrt_ab = sqrt(a*b);
            if (sqrt_ab * sqrt_ab != a*b)
                continue;
            ui _d = a+b+2*sqrt_ab;
            ui _n = a*b;
            if (_n % _d != 0)
                continue;
            ui c = _n/_d;
            ans += a + b + c;
            ui g = gcd(a,(gcd(b,c)));
            ui _a = a/g;
            ui _b = b/g;
            ui _c = c/g;
            ui h = 100000;
            ui hash  = _a * h*h + _b *h + _c;
            if (s.find(hash) == s.end())
                s[hash] = 0;
            s[hash] +=g;
        }
    }
    for (auto p: s)
    {
        ui hash = p.first;
        ui g = p.second;
        ui h = 100000;
        cout << sqrt((hash / (h*h))) << " " << sqrt((hash/h) % h) << " " << (hash % (h)) << " " << g << endl;
    }
    return ans;
}

int main()
{
    ui ans = solve();
    cout << ans << endl;
    return 0;
}
