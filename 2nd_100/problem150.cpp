// Question: https://projecteuler.net/problem=150

#include<iostream>
#include<vector>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N = 1000;

const i64 MOD = 1 << 20;
const i64 DELTA = 1 << 19;

int main()
{
    i64 ans = 0;

    vector<vector<i64>> triangle = vector<vector<i64>>(N, vector<i64>());
    vector<i64> sum = vector<i64>(N, 0);
    vector<vector<i64>> prefix_sum = vector<vector<i64>>(N, vector<i64>());

    i64 t = 0;
    for (i64 row = 0; row < N; row++)
        for (i64 col = 0; col < row+1; col++)
        {
            t = (615949*t + 797807) % MOD;
            triangle[row].push_back(t-DELTA);
            ans = min(ans, triangle[row].back());
        }

    for (i64 row = 0; row < N; row++)
    {
        i64 z = 0;
        prefix_sum[row].push_back(0);
        for (i64 col = 0; col < row + 1; col++)
        {
            z = z + triangle[row][col];
            prefix_sum[row].push_back(z);
        }
        sum[row] = z;
    }

    for (i64 row = 0; row < N; row++)
    {
        for (i64 col = 0; col < row + 1; col++)
        {
            i64 n_rows = 2;
            i64 n_cols = 2;
            i64 triangle_sum = triangle[row][col];
            while ((row + n_rows - 1 < N) && (col + n_cols - 1 < N))
            {
                i64 next_row = row + n_rows - 1;
                i64 next_col = col + n_cols - 1;
                triangle_sum = triangle_sum - prefix_sum[next_row][col] + prefix_sum[next_row][next_col+1];
                n_rows = n_rows + 1;
                n_cols = n_cols + 1;
                ans = min(triangle_sum, ans);
            }
        }
    }

    cout << ans << endl;

    return 0;
}