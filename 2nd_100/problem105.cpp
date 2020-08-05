// Problem: https://projecteuler.net/problem=105

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

constexpr ui pow2(ui n)
{
    if (n == 0)
        return 1;
    return 2 * pow2(n-1);
}

bool check(vector<ui>& v)
{
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
        //min_sums[n_1_bits] = min(min_sums[n_1_bits], sum_1_bits);
        //max_sums[n_1_bits] = max(max_sums[n_1_bits], sum_1_bits);
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

int main()
{
    ui ans = 0;

    vector<ui> A;

    string line;
    ifstream f("../inputs/p105_sets.txt");
    while (getline(f, line))
    {
        A.clear();
        istringstream input_stream = istringstream(line);
        ui t;
        char c;
        while (input_stream >> t)
        {
            A.push_back(t);
            input_stream >> c;
        }
        if (check(A))
            ans += accumulate(A.begin(), A.end(), 0);
    }    
    f.close();    
    cout << ans << endl;

    return 0;
}
