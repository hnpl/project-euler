// Problem: https://projecteuler.net/problem=461

/*
    Find (a, b, c, d) by for each (a, b), find the best (c, d) that minimizes |f(N,a) + f(N,b) + f(N,c) + f(N,d) - pi|.
*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<numeric>

using namespace std;

#define endl "\n"
const double pi = 3.1415926535897932384626433;

const int N = 10000;

// upperbound = k such that for all j <= k, f_n(j) <= pi. So e^(j/n) <= pi + 1.
const int upperbound = int(log(pi+1)*N);
const int M = (upperbound + 1) * (upperbound + 2) / 2;

vector<int> argsort(const vector<double>& vals)
{
    vector<int> idx = vector<int>(vals.size());
    iota(idx.begin(), idx.end(), 0);
    stable_sort(idx.begin(), idx.end(), [&vals](size_t idx1, size_t idx2) {return vals[idx1] < vals[idx2];});
    return idx;
}

// "binary search"
// Optimize |target - val|
int quote_binary_search_endquote(const vector<double>& vals, double target)
{
    int l = 0;
    int r = vals.size() - 1;
    int m = (l + r) / 2;
    while (l < r)
    {
        m = (l + r) / 2;
        if ((vals[m] >= target))
            r = m - 1;
        else
            l = m + 1;
    }

    double best_diff = abs(vals[m] - target);
    int best_m = m;

    // search from m-2 to m+2
    for (int i = m-2; i <= m+2; i++)
    {
        if (i < 0 || i >= vals.size())
            continue;
        double diff = abs(vals[i] - target);
        if (diff < best_diff)
        { 
            best_diff = diff;
            best_m = i;
        }
    }

    return best_m;
}

int main()
{
    int ans = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;

    // assume a <= b and c <= d

    vector<int> keys_x  = vector<int>(M);
    vector<int> keys_y  = vector<int>(M);
    vector<double> vals;
    int n = 0;
    for (int a = 0; a <= upperbound; a++)
        for (int b = a; b <= upperbound; b++)
        {
            double val = exp(1.0*a/N) - 1.0 + exp(1.0*b/N) - 1.0;
            if (val > pi)
                continue;
            keys_x[n] = a;
            keys_y[n] = b;
            vals.push_back(val);
            n = n + 1;
        }

    vector<int> sorted_idx = argsort(vals);
    vector<double> sorted_vals = vector<double>(vals.size());
    for (int i = 0; i < vals.size(); i++)
        sorted_vals[i] = vals[sorted_idx[i]];

    double best_diff = pi;
    int best_pair1 = -1;
    int best_pair2 = -1;
    for (int pair1 = 0; pair1 < sorted_vals.size(); pair1++)
    {
        double val1 = sorted_vals[pair1];
        int pair2 = quote_binary_search_endquote(sorted_vals, pi - val1);
        double diff = abs(val1 + sorted_vals[pair2] - pi);
        if (diff < best_diff)
        {
            best_pair1 = pair1;
            best_pair2 = pair2;
            best_diff = diff;
        }
    }

    a = keys_x[sorted_idx[best_pair1]];
    b = keys_y[sorted_idx[best_pair1]];
    c = keys_x[sorted_idx[best_pair2]];
    d = keys_y[sorted_idx[best_pair2]];


    ans = a*a + b*b + c*c + d*d;

    cout << ans << endl;

    return 0;
}

