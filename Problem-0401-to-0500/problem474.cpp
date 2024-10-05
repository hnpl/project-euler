// Question: https://projecteuler.net/problem=474

#include<iostream>
#include<unordered_map>
#include<vector>
#include<array>
#include<algorithm>
#include<math.h>
#include<fstream>
#include<sstream>
#include<cassert>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

constexpr ui e10(ui n) // return 10**n
{
    if (n == 0)
        return 1;
    return e10(n-1) * 10;
};

const ui N = e10(6);
const ui M = 65432;
const ui M_MOD = e10(ui(log10(M)) + 1);
const ui MOD = e10(16) + 61;

vector<ui> load_primes(ui n) // load primes <= n
{
    ifstream f("../inputs/primes_1e6.txt");
    vector<ui> primes;
    uint64_t p;
    while (f >> p)
    {
        if (p > n)
            break;
        primes.push_back(p);
    }
    f.close();
    return primes;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = load_primes(N);

    ui n_primes = primes.size();

    unordered_map<ui, ui> max_exp;
    for (auto prime: primes)
    {
        max_exp[prime] = 0;
        ui base = prime;
        while (base < N)
        {
            max_exp[prime] += N / base;
            base = base * prime;
        }
    }

    array<ui, M_MOD> array1;
    array1.fill(0);
    array<ui, M_MOD> array2;
    array2.fill(0);

    array<ui, M_MOD>* prev = &array1;
    array<ui, M_MOD>* curr = &array2;

    curr->at(1) = 1;

    for (auto prime: primes)
    {
        cout << (prime) << endl;
        swap(prev, curr);

        curr->fill(0);

        ui period_start = 0;
        ui period_length = 0;

        unordered_map<ui, ui> seen;
        ui base = 1;
        ui exp = 0;
        while (exp <= max_exp[prime])
        {
            if (seen.find(base) != seen.end())
            {
                period_start = seen[base];
                period_length = exp - seen[base];
                break;
            }
            seen[base] = exp;
            base = (base * prime) % M_MOD;
            exp = exp + 1;
        }
        unordered_map<ui, ui> mod_freq_map;
        if (period_length == 0)
        {
            for (auto p: seen)
            {
                auto mod = p.first;
                mod_freq_map[mod] = 1;
            }
        }
        else
        {
            for (auto p: seen)
            {
                auto mod = p.first;
                auto first_exp = p.second;
                if (first_exp < period_start)
                    mod_freq_map[mod] = 1;
                else
                    mod_freq_map[mod] = (max_exp[prime] - first_exp) / period_length + 1;
            }
        }

        for (ui prev_mod = 0; prev_mod < M_MOD; prev_mod++)
        {
            auto prev_freq = prev->at(prev_mod);
            for (auto p2: mod_freq_map)
            {
                auto mod = p2.first;
                auto freq = p2.second;
                auto next_mod = (prev_mod * mod) % M_MOD;
                auto next_freq = (prev_freq * freq) % MOD;
                curr->at(next_mod) = (curr->at(next_mod) + next_freq) % MOD;
            }
        }
    }
    
    ans = curr->at(M);

    cout << ans << endl;

    return 0;
}