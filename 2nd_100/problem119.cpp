// Question: https://projecteuler.net/problem=119

#include<iostream>
#include<cmath>
#include<vector>
#include<algorithm>
#include<string>
#include<sstream>
#include<unordered_map>

using namespace std;

typedef __int128_t int128;

#define endl "\n"

const int128 N = 30;

bool check(int128 digits, int128 expected_sum)
{
    int128 sum = 0;
    while (digits > 0)
    {
        sum = sum + digits % 10;
        digits = digits / 10;
    }

    return sum == expected_sum;
}

string int128_to_string(int128 n)
{
    stringstream stream;

    bool is_negative = n < 0;

    if (is_negative)
        n = -n;

    while (n > 0)
    {
        stream << (char)('0' + (n % 10));
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (is_negative)
        str = "-" + str;

    return str;
}



int main()
{
    vector<int128> a;

    int128 a_upperbound = 10000000;

    unordered_map<int128, int128> exp_upperbound;
    unordered_map<int128, int128> old_base_exp;

    while (a.size() < N)
    {
        int128 base_upperbound = int128(sqrt(a_upperbound));
        for (int128 base = 2; base <= base_upperbound; base++)
        {
            if (exp_upperbound.find(base) == exp_upperbound.end())
            {
                exp_upperbound[base] = 2;
                old_base_exp[base] = base * base;
            }
            int128 exp = exp_upperbound[base];
            int128 product = old_base_exp[base];

            while (product < a_upperbound)
            {
                if (check(product, base))
                    a.push_back(product);
                exp = exp + 1;
                product = product * base;
            }

            exp_upperbound[base] = exp;
            old_base_exp[base] = product;
        }

        a_upperbound = a_upperbound * 10;
    }

    sort(a.begin(), a.end());

    cout << int128_to_string(a[N-1]) << endl;

    return 0;
}

