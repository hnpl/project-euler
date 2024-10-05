// Question:https://projecteuler.net/problem=51

#include<iostream>
#include<vector>
#include<cmath>
#include<unordered_set>
#include<fstream>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 20000000;

vector<ui> get_primes()
{
    vector<ui> v;

    ifstream f("../inputs/primes_1e6.txt");
    ui p;
    while (f >> p)
        if (p > 100000)
            v.push_back(p);
    return v;
}

inline ui replace_path(vector<ui>& path, ui i)
{
    ui ans = 0;
    for (auto n: path)
    {
        if (n == 10)
            ans = ans * 10 + i;
        else
            ans = ans * 10 + n;
    }
    return ans;
}

bool check_path(vector<ui>& path)
{
    for (auto n: path)
        if (n == 10)
            return true;
    return false;
}

inline bool is_prime(unordered_set<ui>& prime_set, ui n)
{
    if (prime_set.find(n) == prime_set.end())
        return false;
    return true;
}

ui check_path_family(unordered_set<ui>& prime_set, vector<ui>& path)
{
    int ans = 0;

    int n_variations = 10;
    if (path[0] == 10)
        n_variations = 9;

    for (int i = 9; i >= 0; i--)
    {
        if (is_prime(prime_set, replace_path(path, i)))
            ans = ans + 1;
        if (ans + i + 1 - n_variations < 0)
            return 0;
    }
    return ans;
}

void dfs(unordered_set<ui>& prime_set, ui& ans, vector<ui>& path, ui depth)
{
    if (depth == 6)
    {
        if (!check_path(path))
            return;
        if (check_path_family(prime_set, path) == 8)
        {
            ui r = 999999;
            if (path[0] == 10)
                r = replace_path(path, 1);
            else
                r = replace_path(path, 0);
            ans = min(r, ans);
        }
        return;
    }
    if (depth == 0)
    {
        for (ui i = 1; i <= 10; i++)
        {
            path[0] = i;
            dfs(prime_set, ans, path, depth+1);
        }
    }
    else
    {
        for (ui i = 0; i <= 10; i++)
        {
            path[depth] = i;
            dfs(prime_set, ans, path, depth+1);
        }
    }
}

int main()
{   
    ui ans = 1000000;

    vector<ui> primes = get_primes();

    unordered_set<ui> prime_set = unordered_set<ui>(primes.begin(), primes.end());

    vector<ui> path = vector<ui>(6, 0);

    dfs(prime_set, ans, path, 0);

    cout << ans << endl;

    return 0;
}