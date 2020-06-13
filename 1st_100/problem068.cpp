// Problem: https://projecteuler.net/problem=68

#include<iostream>
#include<algorithm>
#include<vector>
#include<numeric>
#include<sstream>

using namespace std;

#define endl "\n"

typedef uint64_t N64;

const N64 N = 10;

const N64 M = 16;

bool V1_lowest(const vector<N64>& V)
{
    for (int i = 1; i < N/2; i++)
        if (V[0] > V[i])
            return false;
    return true;
}

string V_to_number(const vector<N64>& V)
{
    stringstream s;
    N64 n = V[0] + V[5] + V[6];
    if (V[1] + V[6] + V[7] != n ||
        V[2] + V[7] + V[8] != n ||
        V[3] + V[8] + V[9] != n ||
        V[4] + V[9] + V[5] != n)
        return "0";
    s << V[0] << V[5] << V[6];
    s << V[1] << V[6] << V[7];
    s << V[2] << V[7] << V[8];
    s << V[3] << V[8] << V[9];
    s << V[4] << V[9] << V[5];

    if (s.str().size() != M)
        return "0";
    return s.str();
}

int main()
{
    string ans = "0";

    vector<N64> V = vector<N64>(N);

    iota(begin(V), end(V), 1);

    do
    {
        if (!V1_lowest(V))
            continue;
        string n = V_to_number(V);
        if (n.compare(ans) > 0)
            ans = n;
    } while (next_permutation(V.begin(), V.end()));

    cout << ans << endl;

    return 0;
}