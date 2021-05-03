// Question: https://projecteuler.net/problem=341

/*
    - G(n): the n^th element of the Golomb's self-describing sequence
    - L(n): number of number "n" in the sequence of G(k)
    - M(n): number of L(k) such that of length n

    Observation: M(n) is also a Golomb's self-describing sequence
*/


#include<iostream>
#include<map>
#include<algorithm>
#include<cassert>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

constexpr ui POW10(ui n)
{
    if (n == 0)
        return 1;
    return POW10(n-1) * 10;
};

const ui N = POW10(6);

ui getGn(const map<ui, ui>& G, const ui& n) // find that largest key that <= n
{
    auto it = G.upper_bound(n);
    if (it != G.begin())
        --it;
    return G.at(it->first);
}

ui sum(const ui& from, const ui& to)
{
    ui n_numbers = to - from + 1;
    return n_numbers * from + (1+n_numbers) * n_numbers / 2 - n_numbers;
}

/* // Original version
ui getGnFast(const map<ui, ui>& G, const ui& target_n)
{
    ui gn = 0;

    ui curr_length = 0;
    ui curr_freq = 0;
    ui n = 0;

    for (auto p: G)
    {
        ui next_length = p.first;
        ui next_n = n + sum(curr_length, next_length-1) * curr_freq;
        if (next_n >= target_n)
            break;
        gn += (next_length - curr_length) * curr_freq;
        n = next_n;
        curr_length = next_length;
        curr_freq += 1;
    }

    while (true)
    {
        ui next_n = n + curr_length * curr_freq;
        if (next_n >= target_n)
            break;
        n = next_n;
        gn += curr_freq;
        curr_length += 1;
    }

    gn += (target_n - n - 1) / curr_length + 1;

    return gn;
}
*/

class Cache
{
    public:
        ui curr_length;
        ui curr_freq;
        ui gn;
        ui n;

        Cache()
        {
        }

        Cache(ui curr_length, ui curr_freq, ui gn, ui n)
        {
            this->curr_length = curr_length;
            this->curr_freq = curr_freq;
            this->gn = gn;
            this->n = n;
        }
};

void buildCache(map<ui, Cache>& cache, const ui& n_upperbound, const map<ui, ui>& G)
{
    ui gn = 0;
    ui curr_length = 0;
    ui curr_freq = 0;
    ui n = 0;

    cache[n] = Cache(curr_length, curr_freq, gn, n);

    for (auto p: G)
    {
        ui next_length = p.first;
        ui next_n = n + sum(curr_length, next_length-1) * curr_freq;
        if (next_n >= n_upperbound)
            break;
        gn += (next_length - curr_length) * curr_freq;
        n = next_n;
        curr_length = next_length;
        curr_freq += 1;
        cache[n] = Cache(curr_length, curr_freq, gn, n);
    }
}

ui getGnFast(const map<ui, Cache>& cache, const ui& target_n)
{
    auto it = cache.upper_bound(target_n);
    if (it != cache.begin())
        --it;
    Cache c = cache.at(it->first);

    ui gn = c.gn;
    ui curr_length = c.curr_length;
    ui curr_freq = c.curr_freq;
    ui n = c.n;

    while (true)
    {
        ui next_n = n + curr_length * curr_freq;
        if (next_n >= target_n)
            break;
        n = next_n;
        gn += curr_freq;
        curr_length += 1;
    }

    if (target_n != n)
        gn += (target_n - n - 1) / curr_length + 1;

    return gn;
}

void computeG(map<ui, ui> & G, const ui& target_n)
{
    G[1] = 1;
    G[2] = 2;
    ui curr_n = 2;
    ui curr_Gn = 2;
    for (ui i = 2; i < target_n; i++)
    {
       curr_n += getGn(G, i);
       curr_Gn = curr_Gn + 1;
       G[curr_n] = curr_Gn;
    }
}

int main()
{
    ui ans = 0;

    map<ui, ui> G;
    map<ui, Cache> cache;

    computeG(G, N);
    buildCache(cache, N * N * N, G);

    /* // Visualizing the pattern
    ui prev = 0;
    ui n = 0;
    ui prev_diff = 0;
    ui count = 0;
    cout << "1 -> [ ";
    for (auto p: G)
    {
        ui diff = p.first - prev;
        if (n > 0)
        {
            if (diff == prev_diff)
            {
                cout << n << " ";
                count += 1;
            }
            else
            {
                cout <<"] " << count << endl << diff << " -> [ " << n << " ";
                count = 1;
            }
        }
        prev_diff = diff;
        prev = p.first;
        n++;
    }

    cout << "]" << endl;
    */

    for (ui n = 1; n < POW10(6); n++)
    {
        auto r = getGnFast(cache, n*n*n);
        ans += r;
    }

    cout << ans << endl;

    return 0;
}
