// Question:https://projecteuler.net/problem=51

#include<iostream>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<fstream>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 10000000;

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

bool compare_maps(unordered_map<ui, ui> a, unordered_map<ui, ui> b)
{
    if (!(a.size() == b.size()))
        return false;
    for (auto p: a)
    {
        auto key = p.first;
        auto val = p.second;
        if ((b.find(key) == b.end()) || (b[key] != val))
            return false;
    }
    return true;
}

bool permutations(ui m, ui n)
{
    unordered_map<ui, ui> m_freq;
    unordered_map<ui, ui> n_freq;

    while (m > 0)
    {
        ui digit = m % 10;
        if (m_freq.find(digit) == m_freq.end())
            m_freq[digit] = 0;
        m_freq[digit] = m_freq[digit] + 1;
        m = m / 10;
    }

    while (n > 0)
    {
        ui digit = n % 10;
        if (n_freq.find(digit) == n_freq.end())
            n_freq[digit] = 0;
        n_freq[digit] = n_freq[digit] + 1;
        n = n / 10;
    }

    return compare_maps(m_freq, n_freq);
}


int main()
{   
    ui ans = 87109;
    double min_ratio = 87107.0/79180.0;
    
    vector<ui> phi = get_totient(N);

    for (ui n = 2; n < N; n++)
        if (permutations(n, phi[n]))
        {
            double ratio = n*1.0/phi[n];
            if (ratio < min_ratio)
            {
                min_ratio = ratio;
                ans = n;
            }
        }

    cout << ans << endl;

    return 0;
}