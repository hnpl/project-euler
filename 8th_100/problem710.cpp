// Question: https://projecteuler.net/problem=710

/*
 * . Essentially, we can build partitions of sum N by adding k on both sides of each partition of partitions of sum N - 2*k. Naturally, we can only build paritions with an even sum from paritions with an even sum.
 * . We need to separate the count of partitions containing the number 2, and the paritions do not have it.
 * . Because when we build partitions of N, we need to access partitions of N - 2*k for all k, we can keep track of the accumulate sum of N-2*k for all k without recalculating it for every N.
 */

#include<iostream>
#include<vector>
#include<array>
#include<algorithm>
#include<cassert>

using namespace std;

typedef int64_t ui; // using signed integers to avoid modulus arithmetic underflow
#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui UPPERBOUND = pow(10, 7);
const ui MOD = pow(10, 6); // we are finding t(n) that is divisible by one million, so we only care about t(n) % 10**6, not the value of t(n) itself

ui solve(const ui& upperbound)
{
    ui ans = 0;
    
    vector<ui> DP_no_twopals = vector<ui>(upperbound+1);
    vector<ui> DP_twopals = vector<ui>(upperbound+1);

    vector<ui> acc_sum_DP_no_twopals = vector<ui>(upperbound+1);
    vector<ui> acc_sum_DP_twopals = vector<ui>(upperbound+1);

    DP_no_twopals[0] = 1;
    DP_no_twopals[1] = 1; // (1)
    DP_no_twopals[2] = 1; // (1,1)
    DP_no_twopals[3] = 2; // (1,1,1), (3)
    DP_no_twopals[4] = 2; // (1,1,1,1), (4)
    DP_twopals[0] = 0;
    DP_twopals[1] = 0;
    DP_twopals[2] = 1; // (2)
    DP_twopals[3] = 0;
    DP_twopals[4] = 2; // (1,2,1), (2,2)


    acc_sum_DP_no_twopals[0] = DP_no_twopals[0];
    acc_sum_DP_no_twopals[1] = DP_no_twopals[1];
    acc_sum_DP_no_twopals[2] = DP_no_twopals[0] + DP_no_twopals[2];
    acc_sum_DP_no_twopals[3] = DP_no_twopals[1] + DP_no_twopals[3];
    acc_sum_DP_no_twopals[4] = DP_no_twopals[0] + DP_no_twopals[2] + DP_no_twopals[4];

    acc_sum_DP_twopals[0] = DP_twopals[0];
    acc_sum_DP_twopals[1] = DP_twopals[1];
    acc_sum_DP_twopals[2] = DP_twopals[0] + DP_twopals[2];
    acc_sum_DP_twopals[3] = DP_twopals[1] + DP_twopals[3];
    acc_sum_DP_twopals[4] = DP_twopals[0] + DP_twopals[2] + DP_twopals[4];

    ui n = 5;

    while (n <= upperbound)
    {
        /*
        ui count_no_twopals = 1;
        ui count_twopals = 0;

        for (ui next_element = 1; next_element <= n / 2; next_element++)
        {
            ui prev_sum = n - next_element * 2;
            if (next_element == 2)
            {
                count_twopals += DP_no_twopals[prev_sum] + DP_twopals[prev_sum];
                count_twopals %= MOD;
            }
            else
            {
                count_twopals += DP_twopals[prev_sum];
                count_twopals %= MOD;
                count_no_twopals += DP_no_twopals[prev_sum];
                count_no_twopals %= MOD;
            }
        }
        */
        ui count_no_twopals = (acc_sum_DP_no_twopals[n-2] - DP_no_twopals[n-4] + 1) % MOD; // n-4 means we add the number 2 to both sides of each partition, and thus, those partitions become twopal partitions.
        ui count_twopals = (acc_sum_DP_twopals[n-2] + DP_no_twopals[n-4]) % MOD;

        DP_no_twopals[n] = count_no_twopals;
        DP_twopals[n] = count_twopals;
        acc_sum_DP_no_twopals[n] = (DP_no_twopals[n] + acc_sum_DP_no_twopals[n-2]) % MOD;
        acc_sum_DP_twopals[n] = (DP_twopals[n] + acc_sum_DP_twopals[n-2]) % MOD;

        if (count_twopals == 0 && n > 42)
            return n;

        if (n % 10000 == 0)
            cout << n << endl;

        n++;
    }

    return ans;
}

int main()
{
    ui ans = solve(UPPERBOUND);
    cout << ans << endl;
    return 0;
}
