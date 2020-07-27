// Question: https://projecteuler.net/problem=250

// g++ problem250.cpp -L$HOME/pari/lib -lpari -I$HOME/pari/include -O3

/*
    First, precalculate i**i mod 250 for all i <= 250250.
    Let DP[i][j] be the number of subsets of {1**1, 2**2, ..., i**i} such that each subset mod 250 = 0.
    Then, let k = (i+1)**(i+1) mod 250, we have
        DP[i+1][(j+k) mod 250] = DP[i][(j+k) mod 250] + DP[i][j mod 250]
                                 ^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^
                                 choose to add i+1      choose not to add i+1
                                 to the subset          to the subset           
*/

#include<pari/pari.h>
#include<iostream>
#include<vector>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 250250;
const ui M = 250;
const ui LAST_DIGITS_MOD = 10'000'000'000'000'000ULL;

int main()
{

    pari_init(100000000,2);

    ui ans = 0;

    vector<ui> modM = vector<ui>(N+1);
    for (ui i = 0; i <= N; i++)
        modM[i] = gtolong(gel(gpowgs(gmodulss(i, 250), i),2));


    ui DP[2][M];
    for (ui i = 0; i < M; i++)
        DP[0][i] = 0;
    DP[0][0] = 1;
    DP[0][1] = 1;

    ui curr_i = 0;
    ui prev_i = 1; 

    for (ui i = 2; i <= N; i++)
    {
        curr_i = (i+1)%2;
        prev_i = 1 - curr_i;
        ui mod = modM[i];

        for (ui j = 0; j < M; j++)
            DP[curr_i][j] = DP[prev_i][j];
        for (ui j = 0; j < M; j++)
            DP[curr_i][(j+mod)%M] = (DP[curr_i][(j+mod)%M] + DP[prev_i][j]) % LAST_DIGITS_MOD;
    }

    ans = DP[curr_i][0] - 1;

    cout << ans << endl;

    pari_close();

    return 0;
}
