// Question: Question: https://projecteuler.net/problem=95

#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<unordered_set>
#include<algorithm>
#include<numeric>

using namespace std;

typedef int64_t int64;

const int64 N = 1000000;

int main()
{
    unordered_set<int64> primes;
    ifstream f("../inputs/primes_1e6.txt");
    int64 prime;
    while (f >> prime)
        primes.insert(prime);
    f.close();

    unordered_set<int64> primes2;
    int64 sq = int(sqrt(N));
    for (auto p: primes)
        if (p <= sq)
            primes2.insert(p);

    vector<int64> sum_of_divisors(N+1, 0);
    sum_of_divisors[1] = 1;

    // if a is prime then sum_of_divisors[a] = 1 + a
    for (auto p: primes)
    {
        sum_of_divisors[p] = p + 1;
        int64 j = p;
        while (j*p <= N)
        {
            sum_of_divisors[j*p] = sum_of_divisors[j] + j*p;
            j = j*p;
        }
    }

    for (int64 i = 2; i <= N; i++)
    {
        if (sum_of_divisors[i] > 0)
            continue;
        for (auto p: primes2)
        {
            if (i % p == 0)
            {
                // if a and b are coprime then sum_of_divisors[a*b] = sum_of_divisors[a] * sum_of_divisors[b]
                int64 a = i / p;
                int64 b = p;
                while (a % p == 0)
                {
                    a = a / p;
                    b = b * p;
                }
                sum_of_divisors[i] = sum_of_divisors[a] * sum_of_divisors[b];
                break;
            }
        }
    }

    auto arange = std::vector<int64>(N+1);
    iota(arange.begin(), arange.end(), 0);
    transform(sum_of_divisors.begin(), sum_of_divisors.end(), arange.begin(), sum_of_divisors.begin(), minus<int64>());
    sum_of_divisors[1] = 1;

    int64 ans = 0;
    int64 max_length = 0;
    vector<bool> checked = vector<bool>(N+1, false);

    for (int64 i = 2; i <= N; i++)
    {
        if (checked[i])
            continue;
        
        vector<int64> chain;
        unordered_set<int64> chain_set;
        int64 w = i;
        while (w <= N && chain_set.find(w) == chain_set.end() && w > 1 && !checked[w])
        {
            chain.push_back(w);
            chain_set.insert(w);
            checked[w] = true;
            w = sum_of_divisors[w];
        }
        auto it = find(chain.begin(), chain.end(), w);
        if (w <= N && it != chain.end())
        {
            auto chain_length = distance(it, chain.end());
            auto min_e = min_element(it, chain.end());
            if (chain_length > max_length)
            {
                max_length = chain_length;
                ans = *min_e;
            }
        }
    }

    cout << ans << "\n";

    return 0;
}