// Question: https://projecteuler.net/problem=654
/*
 * . We can construct a tuple by adding elements one-by-one.
 * . We only need to keep track of the last element in the tuple to determine what are possible next states.
 *      . If the last element is K, then the next element could be any number in [1..N-K-1].
 * . We can define a state as the last element.
 * . The transition matrix for N = 10 looks like,
 * array([[0, 0, 0, 0, 0, 0, 0, 0, 0, 0],  // (..., 0)
 *        [1, 1, 1, 1, 1, 1, 1, 1, 1, 1],  // (..., 1)
 *        [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],  // (..., 2)
 *        [1, 1, 1, 1, 1, 1, 1, 1, 0, 0],  // (..., 3)
 *        [1, 1, 1, 1, 1, 1, 1, 0, 0, 0],  // (..., 4)
 *        [1, 1, 1, 1, 1, 1, 0, 0, 0, 0],  // (..., 5)
 *        [1, 1, 1, 1, 1, 0, 0, 0, 0, 0],  // (..., 6)
 *        [1, 1, 1, 1, 0, 0, 0, 0, 0, 0],  // (..., 7)
 *        [1, 1, 1, 0, 0, 0, 0, 0, 0, 0],  // (..., 8)
 *        [1, 1, 0, 0, 0, 0, 0, 0, 0, 0]]) // (..., 9)
 * . If we can find the characteristic of the matrix, then we can avoid doing full matrix exponentiation.
 *   . I haven't found it, so we'll do matrix exponentitation.
 */

#include<iostream>
#include<array>
#include<vector>
#include<iomanip>
#include<thread>
#include<fstream>
#include<string>
#include<algorithm>
#include<sstream>

using namespace std;

typedef __uint128_t ui;

const ui N = 5000;
const ui n_threads = 80;
const ui MOD = 1'000'000'007;
const ui M = 1'000'000'000'000ULL;
//const ui N = 10;
//const ui n_threads = 1;
//const ui MOD = 1000000007;
//const ui M = 100;

typedef vector<array<ui, N>> Matrix;

#define endl "\n"

constexpr ui pow10(ui exponent)
{
    if (exponent == 0)
        return 1;
    return 10 * pow10(exponent-1);
};

string ui_to_string(ui n)
{
    stringstream stream;
    while (n > 0)
    {
        char r = n % 10;
        stream << char('0' + r);
        n = n / 10;
    }
    string str = stream.str();
    reverse(str.begin(), str.end());
    return str;
} 

void MatrixPrint(Matrix& mm)
{
    ui n = mm.size();
    for (ui i = 0; i < n; i++)
    {
        for (ui j = 0; j < n; j++)
            cout << setw(2) << (int)mm[i][j] << " ";
        cout << endl;
    }    
}

void MatrixInit(Matrix& mm)
{
    for (ui i = 0; i < N; i++)
        mm.push_back(array<ui, N>());
}

void MatrixZero(Matrix& mm)
{
    for (auto& row: mm)
        fill(row.begin(), row.end(), 0);
}

void MatrixCopy(Matrix& mm_in, Matrix& mm_out)
{
    ui n = mm_in.size();
    for (ui i = 0; i < n; i++)
        for (ui j = 0; j < n; j++)
            mm_out[i][j] = mm_in[i][j];
}

void MatrixMod(Matrix& mm, ui mod)
{
    ui n = mm.size();
    for (auto& row: mm)
        for (ui i = 0; i < n; i++)
            row[i] = row[i] % mod;
}

void MatrixTranspose(Matrix& mm_in, Matrix& mm_out)
{
    ui n = mm_in.size();
    for (ui i = 0; i < n; i++)
        for (ui j = 0; j < n; j++)
            mm_out[j][i] = mm_in[i][j];
}

void MatrixMultiplyWorker(Matrix& mm_a, Matrix& mm_b_transpose, Matrix& mm_out, ui row_lowerbound, ui row_upperbound)
{
    ui n = mm_a.size();
    for (ui row = row_lowerbound; row <= row_upperbound; row++)
    {
        vector<ui> row_result = vector<ui>(n, 0);
        for (ui col = 0; col < n; col++)
            for (ui i = 0; i < n; i++)
                row_result[col] += mm_a[row][i] * mm_b_transpose[col][i];
        for (ui col = 0; col < n; col++)
            mm_out[row][col] = row_result[col];
    }
}

void MatrixMultiply(Matrix& mm_a, Matrix& mm_b, Matrix& mm_out)
{
    ui n = mm_a.size();

    Matrix mm;
    MatrixInit(mm);
    MatrixZero(mm);
    
    Matrix mm_b_transpose;
    MatrixInit(mm_b_transpose);
    MatrixTranspose(mm_b, mm_b_transpose);

    vector<thread> threads;
    ui rows_per_job = n / n_threads;
    ui row_lowerbound = 0;
    ui row_upperbound = row_lowerbound + rows_per_job;
    for (ui i = 0; i < n_threads; i++)
    {
        thread new_thread(MatrixMultiplyWorker, ref(mm_a), ref(mm_b_transpose), ref(mm), row_lowerbound, row_upperbound);
        threads.push_back(move(new_thread));
        row_lowerbound = row_upperbound + 1;
        row_upperbound = min(row_lowerbound + rows_per_job, n-1);
    }

    for (ui i = 0; i < n_threads; i++)
        threads[i].join();

    MatrixCopy(mm, mm_out);
}

void MatrixPowMod(Matrix& mm_in, Matrix& mm_out_, ui exponent, ui mod)
{
    ui n = mm_in.size();

    Matrix mm_out;
    MatrixInit(mm_out);
    MatrixZero(mm_out);

    Matrix base_matrix;
    MatrixInit(base_matrix);
    MatrixCopy(mm_in, base_matrix);
    MatrixMod(base_matrix, mod);

    bool initialized_mm_out = false;
    while (exponent > 0)
    {
        cout << ui_to_string(exponent) << endl;
        if (exponent % 2 == 1)
        {
            if (!initialized_mm_out)
            {
                MatrixCopy(base_matrix, mm_out);
                initialized_mm_out = true;
            }
            else
            {
                MatrixMultiply(base_matrix, mm_out, mm_out);
                MatrixMod(mm_out, mod);
            }
        }
        exponent = exponent / 2;
        MatrixMultiply(base_matrix, base_matrix, base_matrix);
        MatrixMod(base_matrix, mod);
    }

    MatrixCopy(mm_out, mm_out_);
}

int main()
{
    Matrix T;
    Matrix T_out;

    MatrixInit(T);
    MatrixInit(T_out);

    for (ui prev = 1; prev < N; prev++)
        for (ui next = 1; next <= N-prev; next++)
            T[next][prev] += 1;
    for (ui i = 1; i < N; i++)
        T[i][0] = 1;
    MatrixPowMod(T, T_out, M, MOD);

    ui ans = 0;
    for (ui i = 0; i < N; i++)
        ans += T_out[i][0];

    cout << ui_to_string(ans) << endl;
    cout << ui_to_string(ans % MOD) << endl;

    return 0;
}
