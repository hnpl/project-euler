// Problem: https://projecteuler.net/problem=172

#include<iostream>
#include<vector>
#include<unordered_set>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 18;          // number of digits
const ui M = 3;           // maximum number of occurrences of each digit
const ui DIGIT_BASE = 10; // DIGIT_BASE = 10 -> base 10 numbers

inline ui encode_state(const vector<ui>& k)
{
    ui ans = 0;
    for (auto n: k)
    {
        ans <<= 2;
        ans |= n;
    }
    return ans;
}

inline vector<ui> decode_state(ui n)
{
    vector<ui> v = vector<ui>(DIGIT_BASE);
    for (ui i = 0; i < v.size(); i++)
    {
        v[DIGIT_BASE - i - 1] = n & 0b11;
        n >>= 2;
    }
    return v;
}

constexpr ui pow(ui base, ui exponent)
{
    ui ans = 1;

    ui coeff = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            ans = ans * coeff;
        exponent = exponent / 2;
        coeff = coeff * coeff;
    }

    return ans;
}

int main()
{
    ui ans = 0;

    const ui max_state = pow(2, 20) - 1;

    // DP[i][j] -> number of ways of constructing numbers of length i with digit frequencies encoded in j
    // j \in states[i] -> digit frequencies j where the sum of frequencies is i

    vector<vector<ui>> DP = vector<vector<ui>>(2, vector<ui>(max_state+1, 0));
    vector<unordered_set<ui>> states = vector<unordered_set<ui>>(2, unordered_set<ui>());
    
    states[0].insert(0);
    DP[0][0] = 1;

    ui curr_row = 0;
    ui prev_row = 1;

    for (ui n_digits = 1; n_digits <= N; n_digits++)
    {
        curr_row = n_digits%2;
        prev_row = 1 - curr_row;
        states[curr_row].clear();
        for (auto prev_state: states[prev_row])
        {
            vector<ui> freq = decode_state(prev_state);
            for (ui digit = 0; digit <= 9; digit++)
            {
                if (freq[digit] < M) // avoiding having a digit frequency > 3
                {
                    freq[digit] += 1;
                    ui curr_state = encode_state(freq);
                    DP[curr_row][curr_state] += DP[prev_row][prev_state];
                    states[curr_row].insert(curr_state);
                    freq[digit] -= 1;
                }
            }
        } 
    }

    // calculating total number of ways of constructing 18-digit numbers as described in the problem statement BUT
    // including numbers with a leading zero
    for (auto curr_state: states[curr_row])
        ans = ans + DP[curr_row][curr_state];

    // subtracting the number of ways of constructing 18-digit numbers as described in the problem statement BUT
    // wiith the first digit being 0
    for (auto prev_state: states[prev_row])
    {
        vector<ui> freq = decode_state(prev_state);
        if (freq[0] < M) // we always have the first digit being zero, so the other 17 digits must have fewer than 2 zeros
            ans = ans - DP[prev_row][prev_state];
    }

    cout << ans << endl;

    return 0;
}
