// Question: https://projecteuler.net/problem=147

#include<iostream>
#include<vector>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N = 47;
const i64 M = 43;

void draw_diagonal(i64 i_row, i64 i_col, i64 length, vector<vector<bool>>& grid)
{
    for (i64 i = 0; i < length; i++)
        grid[i_row-i][i_col+i] = true;
}

void draw_grid(const vector<vector<bool>>& grid)
{
    auto n = grid.size();
    auto m = grid[0].size();
    for (i64 i = 0; i < n; i++)
    {
        for (i64 j = 0; j < m; j++)
            if (grid[i][j])
                cout << "*";
            else
                cout << "-";
        cout << endl;
    }
}

void draw_diagonals(vector<vector<bool>>& grid, i64 n, i64 m)
{
    i64 row = n;
    i64 col = 0;
    draw_diagonal(row, col, n-1, grid);
    row = row + 1;
    for (i64 i = 0; i < m-1; i++)
    {
        draw_diagonal(row, col, n, grid);
        draw_diagonal(row, col+1, n-1, grid);
        row = row + 1;
        col = col + 1;
    }
}

i64 check_rectangle(i64 row1, i64 col1, i64 row2, i64 col2, const vector<vector<bool>>& grid)
{
    for (i64 row = row1; row <= row2; row++)
        for (i64 col = col1; col <= col2; col++)
            if (!grid[row][col])
                return 0;
    return 1;
}

i64 count_rectangles(i64 d_row, i64 d_col, const vector<vector<bool>>& grid)
{
    i64 ans = 0;

    i64 n = grid.size();
    i64 m = grid[0].size();

    for (i64 row = 0; row <= n-d_row; row++)
        for (i64 col = 0; col <= m-d_col; col++)
            ans = ans + check_rectangle(row, col, row+d_row-1, col+d_col-1, grid);

    if (d_row != d_col)
        ans = ans * 2;

    return ans;
}


int main()
{
    i64 ans = 0;

    for (i64 m = 1; m <= M; m++)
        for (i64 n = m; n <= N; n++)
        {
            vector<vector<bool>> grid = vector<vector<bool>>(2*n);
            for (i64 i = 0; i < 2*n; i++)
                grid[i] = vector<bool>(2*n, false);
            i64 delta = n*(n+1)*m*(m+1)/4;

            draw_diagonals(grid, n, m);

            for (i64 d_row = 1; d_row <= 2*n; d_row++)
            {
                i64 prev_delta = delta;
                for (i64 d_col = d_row; d_col <= 2*n; d_col++)
                {
                    i64 new_rectangles = count_rectangles(d_row, d_col, grid);
                    if (new_rectangles == 0)
                        break;
                    delta = delta + new_rectangles;
                }
                if (delta == prev_delta)
                    break;
            }
            
            cout << n << " " << m << " " << delta << endl;

            if (n != m && n <= M && m <= N)
                delta = delta * 2;

            ans = ans + delta;
        }
    
    cout << ans << endl;

    return 0;
}