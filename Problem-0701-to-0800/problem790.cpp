// Problem: https://projecteuler.net/problem=790

// Too lazy to implement segment tree so I went with an array-like data structure and sweep line algorithm

#include <iostream>
#include <stdint.h>
#include <vector>
#include <array>
#include <algorithm>
#include <utility>
#include <map>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui SEED = 290797;
const ui L = 50515093;
const ui MOD = 50515093;
const ui N = 100000;

ui curr_rn = SEED;

typedef array<ui, 4> Rectangle;

Rectangle get_next_rect()
{
    Rectangle v;
    v[0] = curr_rn;
    v[1] = (curr_rn * curr_rn) % MOD;
    v[2] = (v[1] * v[1]) % MOD;
    v[3] = (v[2] * v[2]) % MOD;
    curr_rn = (v[3] * v[3]) % MOD;
    if (v[0] > v[1])
        swap(v[0], v[1]);
    if (v[2] > v[3])
        swap(v[2], v[3]);
    return v;
}

void print_rect(const Rectangle& rect)
{
    cout << rect[0] << " " << rect[1] << " " << rect[2] << " " << rect[3] << endl;
}

void print_bucket(const map<int64_t, ui>& bucket)
{
    for (auto p: bucket)
        cout << p.first << " " << p.second << endl;
}

void add_to_buckets(map<int64_t, ui>& buckets, ui segment_start, ui segment_end)
{
    auto l = buckets.lower_bound(segment_start);
    if (l->first != segment_start)
        buckets[segment_start] = prev(l)->second;
    auto r = buckets.lower_bound(segment_end+1);
    if (r->first != segment_end+1)
        buckets[segment_end+1] = prev(r)->second;
    auto it = buckets.lower_bound(segment_start);
    while (it->first <= segment_end)
    {
        it->second = ((it->second)+1)%12;
        it = next(it);
    }
}

void remove_from_buckets(map<int64_t, ui>& buckets, ui segment_start, ui segment_end)
{
    auto it = buckets.lower_bound(segment_start);
    while (it->first <= segment_end)
    {
        it->second = (it->second)-1;
        if (it->second == -1ULL)
            it->second = 11;
        it = next(it);
    }
}

int main()
{
    vector<Rectangle> rects;
    rects.reserve(N);
    for (ui i = 0; i < N; i++)
        rects.push_back(get_next_rect());
    for (auto r: rects)
        print_rect(r);

    vector<pair<ui, ui>> x_starts;
    x_starts.reserve(N);
    for (ui i = 0; i < N; i++)
        x_starts.push_back(make_pair(rects[i][0], i));
    sort(x_starts.begin(), x_starts.end(), [](auto &left, auto &right) {
        return left.first < right.first;        
    });

    vector<pair<ui, ui>> x_ends;
    x_ends.reserve(N);
    for (ui i = 0; i < N; i++)
        x_ends.push_back(make_pair(rects[i][1], i));
    sort(x_ends.begin(), x_ends.end(), [](auto &left, auto &right) {
        return left.first < right.first;
    });

    ui prev_x = 0;
    map<int64_t, ui> buckets;
    buckets[-1] = 0;
    buckets[0] = 11;
    buckets[L] = 0;
    ui i = 0;
    ui j = 0;
    ui ans = 0;
    while ((i < N) || (j < N))
    {
        if ((i < N) && (x_starts[i].first < x_ends[j].first))
        {
            ui prev_key = 0;
            auto it = buckets.begin();
            it = next(it);
            auto prev_it = buckets.begin();
            for (; it != buckets.end(); it++, prev_it++)
            {
                ui key = it->first;
                ui n_layers = prev_it->second;
                ans += (n_layers + 1) * (key - prev_key) * (x_starts[i].first - prev_x);
                //cout << "-> (" << n_layers << "+1) * (" << key << "-" << prev_key << ") * (" << x_starts[i].first << "-" << prev_x << ")"<<endl;
                prev_key = key;
            }
            //update buckets
            ui rect_idx = x_starts[i].second;
            Rectangle rect = rects[rect_idx];
            add_to_buckets(buckets, rect[2], rect[3]);
            prev_x = x_starts[i].first;
            i += 1;
        }
        else if (j < N)
        {
            ui prev_key = 0;
            auto it = buckets.begin();
            it = next(it);
            auto prev_it = buckets.begin();
            for (; it != buckets.end(); it++, prev_it++)
            {
                ui key = it->first;
                ui n_layers = prev_it->second;
                ans += (n_layers + 1) * (key - prev_key) * (x_ends[j].first - prev_x + 1);
                //cout << "--> (" << n_layers << "+1) * (" << key << "-" << prev_key << ") * (" << x_ends[j].first << "-" << prev_x << "+1)"<<endl;
                prev_key = key;
            }
            // update buckets
            ui rect_idx = x_ends[j].second;
            Rectangle rect = rects[rect_idx];
            remove_from_buckets(buckets, rect[2], rect[3]);
            prev_x = x_ends[j].first + 1;
            j += 1;
        }
    }

    ans += 12 * L * (L - prev_x);

    cout << ans << endl;

    return 0;
}
