// Question: https://projecteuler.net/problem=112

#include<iostream>
#include<unordered_map>

using namespace std;

typedef int64_t i64;

#define endl "\n"

inline i64 get_prefix(i64 n)
{
    return n/10;
}

bool is_bouncy(i64 n, unordered_map<i64, i64>& cache) 
// cache[n] = 0 -> n is an increasing number
// cache[n] = 1 -> n is an decreasing number
// cache[n] = 2 -> n is either an increasing number / a decreasing number (eg. 111, 22222)
{
    i64 prefix = get_prefix(n);
    auto it = cache.find(prefix);
    if (it == cache.end())
        return true;
    i64 state = it->second;
    i64 prefix_suffix = prefix % 10;
    i64 n_suffix = n % 10;
    if (state == 0 && prefix_suffix <= n_suffix)
    {
        cache[n] = 0;
        return false;
    }
    else if (state == 1 && prefix_suffix >= n_suffix)
    {
        cache[n] = 1;
        return false;
    }
    else if (state == 2)
    {
        if (prefix_suffix == n_suffix)
            cache[n] = 2;
        else
            cache[n] = 0 + (i64)(prefix_suffix >= n_suffix);
        return false;
    }
    return true;
}

int main()
{
    i64 ans = 0;

    i64 n = 9;

    i64 n_non_bouncy = 9;

    unordered_map<i64, i64> cache;
    for (i64 i = 1; i <= 9; i++)
        cache[i] = 2;

    while (100*n_non_bouncy != n)
    {
        n = n + 1;
        if (!is_bouncy(n, cache))
            n_non_bouncy = n_non_bouncy + 1;
    }
    ans = n;

    cout << ans << endl;

    return 0;
}