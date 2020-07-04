// Question:https://projecteuler.net/problem=75

// g++ problem075.cpp --std=c++17 -O3 -march=native

#include<iostream>
#include<vector>
#include<numeric>
#include<cmath>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui L = 1500000;

// Pythagorean triplet (a,b,c)
// a = m^2 - n^2
// b = 2mn
// c = m^2 + n^2
// where gcd(m,n)=1 and m+n is odd

// Assume m > n,
//    a + b + c = 2m^2 + 2mn > 2n^2 + 2n^2 = 4n^2
//    -> n < sqrt(L/4)

// On the other hand,
//      a+c < L
// -> 2*m^2 < L
// -> m^2 < L/2
// -> m < sqrt(L/2)
int main()
{   
    ui ans = 0;

    ui n_upperbound = (ui)sqrt(L)/2;
    ui m_upperbound = (ui)sqrt(L/2);

    vector<ui> freq = vector<ui>(L+1, 0);

    for (ui n = 1; n <= n_upperbound; n++)
    {
        for (ui m = n+1; m <= m_upperbound; m++)
        {
            if ((m+n)%2 == 1 && gcd(m,n) == 1)
            {
                ui p = 2*m*(m+n);
                for (ui perimeter = p; perimeter <= L; perimeter += p)
                    freq[perimeter] = freq[perimeter] + 1;
            }
        }
    }

    for (auto p: freq)
        if (p == 1)
            ans = ans + 1;

    cout << ans << endl;

    return 0;
}