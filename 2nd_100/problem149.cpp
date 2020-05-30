// Question: https://projecteuler.net/problem=149

// Perform the algorithm for finding the largest contiguous subarray sum on all rows, cols, diagonals and anti-diagonals

#include<iostream>
#include<vector>
#include<algorithm>

#define endl "\n"

typedef int64_t i64;

using namespace std;

i64 DP(vector<i64>::iterator begin, vector<i64>::iterator end, i64 step)
{
    i64 best_sum = 0;
    i64 current_sum = 0;
    bool stop = false;
    for (auto it = begin; !stop; it+=step)
    {
        if (it == end)
            stop = true;
        auto n = *it;
        current_sum = current_sum + n;
        if (current_sum < 0)
            current_sum = 0;
        best_sum = max(best_sum, current_sum);
    }
    return best_sum;
}

int main()
{
    const i64 N = 2000;
    vector<i64> s = vector<i64>(N*N+1);

    for (i64 k = 1; k <= 55; k++)
        s[k] = (100003LL - 200003LL*k + 300007LL*k*k*k) % 1000000LL - 500000LL;
    for (i64 k = 56; k <= 4000000; k++)
        s[k] = (s[k-24] + s[k-55] + 1000000LL) % 1000000LL - 500000LL;

    // s is 1-indexed, now convert it to 0-indexing
    s.erase(s.begin());

    i64 ans = 0;

    // note that 
    // rows
    for (i64 row = 0; row < N; row++)
    {
        auto it_begin = s.begin() + N * row;
        auto it_end = s.begin() + N * (row + 1) - 1;
        auto step = 1;
        ans = max(ans, DP(it_begin, it_end, step));
    }

    // cols
    for (i64 col = 0; col < N; col++)
    {
        auto it_begin = s.begin() + col;
        auto it_end = s.begin() + N * (N-1) + col;
        auto step = N;
        ans = max(ans, DP(it_begin, it_end, step));
    }

    // diagonals
    // (0, i) -> (i, 0)
    for (i64 i = 0; i < N; i++)
    {
        auto it_begin = s.begin() + i;
        auto step = N - 1;
        auto it_end = s.begin() + N * i;
        ans = max(ans, DP(it_begin, it_end, step));
    }
    // (i, N) -> (N, i)
    for (i64 i = 1; i < N; i++)
    {
        auto it_begin = s.begin() + ((i+1)*N - 1);
        auto step = N - 1;
        auto it_end = s.begin() + N * (N-1) + i;
        ans = max(ans, DP(it_begin, it_end, step));
    }

    // anti-diagonals
    // (0, i) -> (N-i, N)
    for (i64 i = 0; i < N; i++)
    {
        auto it_begin = s.begin() + i;
        auto step = N + 1;
        auto it_end = s.begin() + N * (N-i-1) + (N - 1);
        ans = max(ans, DP(it_begin, it_end, step));
    }
    // (i, 0) -> (N, N-i)
    for (i64 i = 1; i < N; i++)
    {
        auto it_begin = s.begin() + (i*N);
        auto step = N + 1;
        auto it_end = s.begin() + (N*(N-1) + N-i-1);
        ans = max(ans, DP(it_begin, it_end, step));
    }

    cout << ans << endl;
    return 0;
}