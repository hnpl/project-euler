// Question: https://projecteuler.net/problem=186

#include<iostream>
#include<unordered_set>
#include<vector>
#include<deque>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N = 1000000;
const i64 PM = 524287;

struct LFG
{
    i64 k;
    deque<i64> prev;
    LFG()
    { 
        k = 0;
        prev.clear(); 
    }
    i64 next()
    {
        k = k + 1;
        if (k <= 55)
        {
            i64 n = (100003 - 200003*k + 300007*k*k*k)%1000000;
            prev.push_back(n);
            return n;
        }
        
        i64 n = (prev[31] + prev[0]) % 1000000;
        prev.pop_front();
        prev.push_back(n);
        return n;
    }
};

struct DisjointSet
{
    vector<i64> set_size;
    vector<i64> set_id;
    i64 size;
    DisjointSet(i64 n)
    {
        size = n;
        set_size = vector<i64>(n, 1);
        set_id = vector<i64>(n);
        for (i64 i = 0; i < n; i++)
            set_id[i] = i;
    }
    i64 Find(i64 v)
    {
        i64 v_root = v;
        while (set_id[v_root] != v_root)
            v_root = set_id[v_root];
        while (set_id[v] != v_root)
        {
            i64 t = set_id[v];
            set_id[v] = v_root;
            v = t;
        }
        return v_root;
    }
    i64 Union(i64 u, i64 v) // return the new root
    {
        u = Find(u);
        v = Find(v);
        if (u == v)
            return u;
        if (set_size[u] < set_size[v])
        {
            set_id[u] = set_id[v];
            set_size[v] = set_size[u] + set_size[v];
            return v;
        }
        else
        {
            set_id[v] = set_id[u];
            set_size[u] = set_size[u] + set_size[v];
            return u;
        }
        return -1;
    }
    i64 Size(i64 v)
    {
        return set_size[Find(v)];
    }
};

int main()
{
    i64 ans = 0;

    LFG number_generator = LFG();

    DisjointSet set(N);
    i64 target = N / 100 * 99;
    while (set.Size(PM) < target)
    {
        ans = ans + 1;
        i64 from;
        i64 to;
        do
        {
            from = number_generator.next();
            to = number_generator.next();
        }
        while (from == to);
        set.Union(from, to);
    }

    cout << ans << endl;

    return 0;
}