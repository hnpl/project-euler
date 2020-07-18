// Question:https://projecteuler.net/problem=351

// Draw 1/6 of the hexagon.
// Let r indicate the row and c indicate the position of the dot in the row.
// All hidden dots are where gcd(r, c) != 1.
// So the number of hidden dots is: # of dots - sum(phi(r) for each row r).

#include<iostream>
#include<algorithm>
#include<numeric>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

const ui N = 100'000'000;

vector<ui> get_totient(ui n)
{
    vector<ui> phi = vector<ui>(n+1, 1);

    for (ui i = 2; i <= n; i++)
    {
        if (phi[i] != 1)
            continue;

        for (ui j = i; j <= n; j+=i)
            phi[j] = (i-1) * phi[j];
        
        for (ui j = i*i; j <= n; j*=i)
        {
            for (ui k = j; k <= n; k += j)
                phi[k] = i * phi[k];
        }
    }
    return phi;
}

string uint128_to_string(ui n)
{
    stringstream stream;

    while (n > 0)
    {
        char r = n % 10;
        stream << char('0' + r);
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    return str;
}

int main()
{
    ui ans = 0;   
    vector<ui> phi = get_totient(N);

    ui sum_phi = 0;
    for (auto it = phi.begin() + 1; it != phi.end(); it++)
        sum_phi = sum_phi + *it;

    ans = 6 * (N*(N+1)/2 - sum_phi);

    cout << uint128_to_string(ans) << endl;

    return 0;
}