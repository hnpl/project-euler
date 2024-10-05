// Question: https://projecteuler.net/problem=217

/*

    . From a number K consisting of n digits: {d_1, d_2, ..., d_n}
        - Let the sum of the left half to be P, and the sum of the right half to be Q.
        - Assume that we know how many numbers of length n and having left sum of P and right sum of Q. Let this number of such numbers to be C_{n,P,Q}.
        - Assume that we know the sum of all numbers of length n and having left sum of P and right sum of Q. Let the sum of such numbers to be S_{n,P,Q}.
        - So,
            S_{n+2, P, Q} = sum_{P-d_l, Q-d_r} {[d_l * (10^(n+1)) + d_r] * C_{n,P-d_l,Q-d_r} + S_{n,P-d_l, Q-d_r} * 10}     [*]
                                 \forall d_l \in [0, 9] and \forall d_r \in [0, 9] such that (P-d_l) >= 0 and (Q-d_r) >= 0.
            why? . S_{n,P-d_l,Q-d_r} is a sum of C_{n,P-d_l,Q-d_r} numbers, and we want to add d_l to the left and d_r to the right of each of those numbers
                 . Suppose one of those numbers is X, after we add d_l to the left and d_r to the right of X, then the number becomes d_l * (10^(n+1)) + X * 10 + d_r
                 . We want to add d_l * (10^(n+1)) + d_r to C_{n,P-d_l,Q-d_r} numbers, hence the new sum becomes [*].
        - Also,
            C_{n+2, P, Q} = sum_{P-d_l, Q-d_r} {C_{n,P-d_l,Q-d_r}}
                                 \forall d_l \in [0, 9] and \forall d_r \in [0, 9] such that (P-d_l) >= 0 and (Q-d_r) >= 0.

*/

#include<iostream>
#include<vector>
#include<unordered_map>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

const ui N = 47;
const ui M = (N/2 + 1) * 9;

constexpr ui pow3(ui n)
{
    if (n == 0)
        return 1;
    return pow3(n-1) * 3;
};

const ui MOD = pow3(15);


ui p10[N+1];

void init()
{
    p10[0] = 1;
    for (ui i = 1; i <= N; i++)
        p10[i] = (p10[i-1] * 10) % MOD;
}

ui count_odd(ui N)
{
    ui ans = 0;

    ui DP_sum[2][M+1][M+1];
    ui DP_count[2][M+1][M+1];
    
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_sum[0][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_sum[1][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_count[0][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_count[1][i][j] = 0;

    ui curr_index = 0;
    ui prev_index = 1;

    for (ui i = 0; i <= 9; i++)
    {
        DP_sum[curr_index][i][i] = i;
        DP_count[curr_index][i][i] = 1;
        ans += i;
    }

    for (ui n_digits = 2; n_digits <= N/2+1; n_digits++)
    {
        // swap the indices
        curr_index = 1 - curr_index;
        prev_index = 1 - prev_index;

        for (ui i = 0; i <= M; i++)
            for (ui j = 0; j <= M; j++)
                DP_sum[curr_index][i][j] = 0;

        for (ui i = 0; i <= M; i++)
            for (ui j = 0; j <= M; j++)
                DP_count[curr_index][i][j] = 0;

        for (ui prev_left_sum = 0; prev_left_sum <= 9*(n_digits-1); prev_left_sum++)
        {
            for (ui left_num = 0; left_num <= 9; left_num++)
            {
                ui left_sum = prev_left_sum + left_num;
                if (left_sum > M)
                    break;
                for (ui prev_right_sum = 0; prev_right_sum <= 9*(n_digits-1); prev_right_sum++)
                {
                    for (ui right_num = 0; right_num <= 9; right_num++)
                    {
                        ui right_sum = prev_right_sum + right_num;
                        
                        if (right_sum > M)
                            break;

                        if (DP_count[prev_index][prev_left_sum][prev_right_sum] == 0)
                            continue;
                        

                        ui factor = (left_num * p10[(n_digits-1)*2] + right_num) % MOD;
                        ui new_sum = (DP_sum[prev_index][prev_left_sum][prev_right_sum] * 10 + DP_count[prev_index][prev_left_sum][prev_right_sum] * factor) % MOD;
                        ui new_count = DP_count[prev_index][prev_left_sum][prev_right_sum];

                        DP_sum[curr_index][left_sum][right_sum] += new_sum;
                        DP_sum[curr_index][left_sum][right_sum] %= MOD;

                        DP_count[curr_index][left_sum][right_sum] += new_count;
                        DP_count[curr_index][left_sum][right_sum] %= MOD;

                        // we don't count the sum of numbers having starting digits of 0.
                        if ((left_num != 0) && (left_sum == right_sum))
                        {
                            ans += new_sum;
                            ans %= MOD;
                        }
                    }
                }
            }
        }
    }

    return ans;
}

ui count_even(ui N)
{
    
    ui ans = 0;

    ui DP_sum[2][M+1][M+1];
    ui DP_count[2][M+1][M+1];
    
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_sum[0][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_sum[1][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_count[0][i][j] = 0;
    for (ui i = 0; i <= M; i++)
        for (ui j = 0; j <= M; j++)
            DP_count[1][i][j] = 0;

    ui curr_index = 0;
    ui prev_index = 1;

    DP_sum[curr_index][0][0] = 0;
    DP_count[curr_index][0][0] = 1;

    for (ui n_digits = 1; n_digits <= N/2; n_digits++)
    {
        // swap the indices
        curr_index = 1 - curr_index;
        prev_index = 1 - prev_index;

        for (ui i = 0; i <= M; i++)
            for (ui j = 0; j <= M; j++)
                DP_sum[curr_index][i][j] = 0;

        for (ui i = 0; i <= M; i++)
            for (ui j = 0; j <= M; j++)
                DP_count[curr_index][i][j] = 0;

        for (ui prev_left_sum = 0; prev_left_sum <= 9*(n_digits-1); prev_left_sum++)
        {
            for (ui left_num = 0; left_num <= 9; left_num++)
            {
                ui left_sum = prev_left_sum + left_num;
                if (left_sum > M)
                    break;
                for (ui prev_right_sum = 0; prev_right_sum <= 9*(n_digits-1); prev_right_sum++)
                {
                    for (ui right_num = 0; right_num <= 9; right_num++)
                    {
                        ui right_sum = prev_right_sum + right_num;
                        
                        if (right_sum > M)
                            break;

                        if (DP_count[prev_index][prev_left_sum][prev_right_sum] == 0)
                            continue;
                        
                        ui factor = (left_num * p10[(n_digits)*2-1] + right_num) % MOD;
                        ui new_sum = (DP_sum[prev_index][prev_left_sum][prev_right_sum] * 10 + DP_count[prev_index][prev_left_sum][prev_right_sum] * factor) % MOD;
                        ui new_count = DP_count[prev_index][prev_left_sum][prev_right_sum];

                        DP_sum[curr_index][left_sum][right_sum] += new_sum;
                        DP_sum[curr_index][left_sum][right_sum] %= MOD;

                        DP_count[curr_index][left_sum][right_sum] += new_count;
                        DP_count[curr_index][left_sum][right_sum] %= MOD;

                        if ((left_num != 0) && (left_sum == right_sum))
                        {
                            ans += new_sum;
                            ans %= MOD;
                        }
                    }
                }
            }
        }
    }
    return ans;
}

int main()
{
    ui ans = 0;

    init(); // initialize pow10

    ans = (count_odd(N) + count_even(N)) % MOD;

    cout << ans << endl;

    return 0;
}