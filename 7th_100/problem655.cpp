// Question: https://projecteuler.net/problem=655

#include<iostream>
#include<algorithm>
#include<vector>

typedef uint64_t ui;
const ui MOD = 10000019;
const ui N = 32;
using namespace std;

int main()
{
    ui p10[N+1]; // power of 10: 10**i
    p10[0] = 1;
    for (ui i = 1; i <= N; i++)
        p10[i] = p10[i-1] * 10;

    ui p10m[N+1]; // power of 10 remainders: 10**i mod MOD
    p10m[0] = 1;
    for (ui i = 1; i <= N; i++)
        p10m[i] = (p10m[i-1] * 10) % MOD;

    ui ans = 0;

    vector<ui> freq_odd = vector<ui>(MOD, 0);
    vector<ui> freq_odd_prev = vector<ui>(MOD);
    // A
    // BAB   -> p=3
    // CBABC -> p=5
    // ...
    for (ui A = 0; A <= 9; A++)
        freq_odd[A % MOD] = 1;
    for (ui p = 3; p <= N; p+=2)
    {
        fill(freq_odd_prev.begin(), freq_odd_prev.end(), 0);

        for (ui i = 0; i < MOD; i++)
            freq_odd_prev[(i*10) % MOD] += freq_odd[i]; // ZAZ = A * 10 + Z0Z
        
        fill(freq_odd.begin(), freq_odd.end(), 0);

        // A -> BAB
        for (ui B = 1; B <= 9; B++) // B is the first digit, so count B = 0 later
        {
            ui delta = (B * (p10m[p-1] + p10m[0])) % MOD;
            // freq_even[(i+delta)%MOD] += freq_even_prev[i]
            for (ui i = 0; i < MOD - delta; i++)
                freq_odd[i + delta] += freq_odd_prev[i]; // map from i -> i + delta
            for (ui i = MOD - delta; i < MOD; i++)
                freq_odd[i + delta - MOD] += freq_odd_prev[i];
        }
       
        ans += freq_odd[0];
        
        ui B = 0;
        ui delta = (B * (p10m[p-1] + p10m[0])) % MOD;
        for (ui i = 0; i < MOD - delta; i++)
            freq_odd[i + delta] += freq_odd_prev[i];
        for (ui i = MOD - delta; i < MOD; i++)
            freq_odd[i + delta - MOD] += freq_odd_prev[i];
    }

    vector<ui> freq_even = vector<ui>(MOD, 0);
    vector<ui> freq_even_prev = vector<ui>(MOD);
    // AA
    // BAAB   -> p=4
    // CBAABC -> p=6
    // ...
    for (ui AA = 0; AA <= 99; AA+=11)
        freq_even[AA % MOD] = 1;
    for (ui p = 4; p <= N; p+=2)
    {
        fill(freq_even_prev.begin(), freq_even_prev.end(), 0);

        for (ui i = 0; i < MOD; i++)
            freq_even_prev[(i*10) % MOD] += freq_even[i]; // ZAAZ = AA * 10 + Z00Z
        
        fill(freq_even.begin(), freq_even.end(), 0);

        // AA -> BAAB
        for (ui B = 1; B <= 9; B++) // B is the first digit, so count B = 0 later
        {
            ui delta = (B * (p10m[p-1] + p10m[0])) % MOD;
            // freq_even[(i+delta)%MOD] += freq_even_prev[i]
            for (ui i = 0; i < MOD - delta; i++)
                freq_even[i + delta] += freq_even_prev[i]; // map from i -> i + delta
            for (ui i = MOD - delta; i < MOD; i++)
                freq_even[i + delta - MOD] += freq_even_prev[i];
        }
        
        ans += freq_even[0];
        
        ui B = 0;
        ui delta = (B * (p10m[p-1] + p10m[0])) % MOD;
        for (ui i = 0; i < MOD - delta; i++)
            freq_even[i + delta] += freq_even_prev[i];
        for (ui i = MOD - delta; i < MOD; i++)
            freq_even[i + delta - MOD] += freq_even_prev[i];
    }

    cout << ans << "\n";

    return 0;
}