// Problem: https://projecteuler.net/problem=662

#include<iostream>
#include<vector>
#include<array>
#include<cmath>
#include<utility>
#include<unordered_set>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N_ROWS = 10000;
const i64 N_COLS = 10000;
const i64 M = 1000000007;

const vector<i64> fibonacci_vals = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946}; // the maximum distance is 10000*sqrt(2)

bool is_square_number(i64 n, i64 &sqrt_n)
{
    bool ans = false;

    sqrt_n = (i64)sqrt(n);

    ans = (sqrt_n * sqrt_n) == n;

    return ans;
}

int main()
{
    i64 ans = 0;

    vector<vector<i64>> DP = vector<vector<i64>>(N_ROWS+1, vector<i64>(N_COLS+1, 0));

    DP[0][0] = 1;

    unordered_set<i64> fibonacci_vals_set;
    for (auto& f1: fibonacci_vals)
        fibonacci_vals_set.insert(f1);

    vector<pair<i64, i64>> pythagorean_fibonacci_pairs;

    for (i64 d_row = 0; d_row <= N_ROWS; d_row++)
    {
        for (i64 d_col = d_row; d_col <= N_COLS; d_col++)
        {
            i64 distance_square = d_row * d_row + d_col * d_col;
            if (distance_square == 0)
                continue;
            i64 distance;
            bool valid_distance = is_square_number(distance_square, distance);
            if (valid_distance)
            {
                if (fibonacci_vals_set.find(distance) != fibonacci_vals_set.end())
                {
                    pythagorean_fibonacci_pairs.push_back(make_pair(d_row, d_col));
                    if (d_row != d_col)
                        pythagorean_fibonacci_pairs.push_back(make_pair(d_col, d_row));
                }
            }
        }
    }

    for (i64 row = 0; row <= N_ROWS; row++)
    {
        for (i64 col = 0; col <= N_COLS; col++)
        {
            for (auto& p: pythagorean_fibonacci_pairs)
            {
                i64 d_row = p.first;
                i64 d_col = p.second;
                i64 prev_row = row - d_row;
                i64 prev_col = col - d_col;
                if (prev_row < 0 || prev_row > N_ROWS || prev_col < 0 || prev_col > N_COLS)
                    continue;
                DP[row][col] += DP[prev_row][prev_col];
            }
            DP[row][col] %= M;
        }
    }

    ans = DP[N_ROWS][N_COLS];

    cout << ans << endl;

    return 0;
}



