// Question: https://projecteuler.net/problem=793

/*
 * Strategy:
 * - Find the sequence S.
 * - Sort the sequence S in place.
 * - Use binary search in the range [0, S[N-1] * S[N]] to find the answer.
 *   - At each value of m, we calculate how many pairs of S_i * S_j that are less than or equal to m where i < j.
 *     - We can do this by fixing S_i, and perform binary search on [i, N) to find the value k such that
 *       . k in [i, N)
 *       . S_i * S*k <= m
 *       . S_i * S_{k+1} > m
 *       -> there'll be k-i-1 pairs of S_i * S_j that are less than m for a particular S_i.
 *   - There are N*(N-1)/2 possible pairs of S_i * S_j.
 *   - We'll find the first value m such that there are exactly N*(N-1)/4 pairs of S_i*S_j that are less than or equal m.
 */

#include<algorithm>
#include<iostream>
#include<cmath>
#include<queue>
#include<vector>
#include<cassert>
#include<numeric>

using namespace std;

#define endl "\n"

const uint64_t seed = 290797;
const uint64_t MOD = 50515093;
const uint64_t N = 1000003;
//const uint64_t N = 103;

uint64_t next_s(uint64_t s)
{
    return (s*s)%MOD;
}

class Tuple3
{
    public:
        double key;
        int i;
        int j;
        Tuple3() { key = 0; i = 0; j = 0;}
        Tuple3(double key, int i, int j)
        {
            this->key = key;
            this->i = i;
            this->j = j;
        }
};

class Comparator
{
    public:
        bool operator()(const Tuple3& left, const Tuple3& right)
        {
            return left.key > right.key;
        }
};

int main()
{
    uint64_t ans_i = 0;
    uint64_t ans_j = 0;
    uint64_t ans = 0;

    vector<uint64_t> S;
    S.reserve(N);
    S.push_back(seed);
    for (int i = 0; i < N-1; i++)
        S.push_back(next_s(S.back()));    
    sort(S.begin(), S.end());

    int64_t l = 0;
    int64_t r = S[S.size()-2] * S[S.size()-1];

    uint64_t target = (N*(N-1)/2+1)/2;

    vector<int64_t> js(N-1);
    vector<int64_t> index(N-1);

    while (l < r)
    {
        int64_t m = (l + r + 1) / 2;
        cout << "l: " << l << "; r: " << r <<"; m:" << m << endl;
        for (int i = 0; i < N - 1; i++)
        {
            int64_t target_j = (m+S[i]-1) / S[i];
            auto next_j_it = lower_bound(S.begin()+i+1, S.end(), target_j);
            if (next_j_it == S.end())
            {
                js[i] = N;
                index[i] = N-i-1;
            }
            else
            {
                js[i] = next_j_it - S.begin()-1;
                index[i] = (next_j_it - S.begin()) - i-1;
            }
        }
        
        /*
        cout << "index [";
        for (auto& i: index)
            cout << i << ",";
        cout << "]" << endl;
        cout << "js [";
        for (auto& i: index)
            cout << i << ",";
        cout << "]" << endl;
        */

        int64_t n_smallers = 0;
        for (auto& i: index)
        {
            n_smallers += i;
        }
        if (n_smallers < target)
        {
            l = m + 1;
        }
        else
        {
            r = m - 1;
        }
    }

    cout << l << " " << r << " " << (l+r)/2 << endl;

    return 0;
}
