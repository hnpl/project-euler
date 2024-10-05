// Problem: https://projecteuler.net/problem=103

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<fstream>
#include<sstream>
#include<string>
#include<unordered_set>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 7;
const ui M = 3;

constexpr ui pow2(ui n)
{
    if (n == 0)
        return 1;
    return 2 * pow2(n-1);
}

bool check(const vector<ui>& V)
{
    vector<ui> v = V;
    ui n = v.size();
    ui max_partition_state = pow2(n)-1;
    ui v_sum = accumulate(v.begin(), v.end(), 0);

    vector<ui> min_sums = vector<ui>(n, -1ULL); // min_sums[i] = the minimum sum of all subsets having the cardinality of i
    vector<ui> max_sums = vector<ui>(n, 0);

    unordered_set<ui> sums;
    sums.insert(0);

    for (ui partition = 1; partition < max_partition_state; partition++)
    {
        ui sum_1_bits = 0;
        ui n_1_bits = 0;
        ui p = partition;
        ui bit_idx = 0;
        while (p > 0)
        {
            bool bit_is_set = p % 2 == 1;
            if (bit_is_set)
            {
                sum_1_bits += v[bit_idx];
                n_1_bits += 1;
            }
            p = p / 2;
            bit_idx += 1;
        }

        if (sums.find(sum_1_bits) != sums.end())
            return false;
        sums.insert(sum_1_bits);
    }

    sort(v.begin(), v.end());
    ui first_k_sum = 0;
    min_sums[1] = v[0];
    max_sums[n-1] = v_sum - min_sums[1];
    for (int i = 1; i < n-1; i++)
    {
        min_sums[i+1] = min_sums[i] + v[i];
        max_sums[n-i-1] = v_sum - min_sums[i+1];
    }    

    for (ui i = 0; i < n-2; i++)
        if (min_sums[i+1] < max_sums[i])
            return false;

    return true;
}

void DFS(ui depth, ui curr_sum, vector<ui>& B, const vector<ui>& A, ui& best_sum, vector<ui>& best_B)
{
    ui n = B.size();

    if (depth == n)
    {
        ui sum = accumulate(B.begin(), B.end(), 0);
    
        if (sum < best_sum && check(B))
        {
            best_sum = sum;
            best_B = B;
        }
    }
    else
    {
        for (ui next = max(B[depth-1]+1, A[depth-1]+B[0]-M); next <= A[depth-1] + B[0] + M; next++)
        {
            B[depth] = next;
            ui next_sum = curr_sum + next;
            if (next_sum > best_sum)
                break;
            DFS(depth+1, curr_sum + next, B, A, best_sum, best_B);
        }
    }
}

int main()
{
    ui best_sum = -1ULL;
    vector<ui> ans = vector<ui>(N);

    const vector<ui> A = {11, 18, 19, 20, 22, 25};

    vector<ui> B = vector<ui>(N);

    for (ui b = 1; b <= 30; b++)
    {
        B[0] = b;
        DFS(1, b, B, A, best_sum, ans);
    }

    for (auto e: ans)
        cout << e;
    cout << endl;

    return 0;
}
