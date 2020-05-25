// Question: https://projecteuler.net/problem=133

#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_set>

using namespace std;

#define endl "\n"

typedef int64_t int64;

const int64 N = 100000;

int main()
{
    int64 ans = 0;

    vector<int64> primes;
    ifstream f("../inputs/primes_1e6.txt");
    int64 prime;
    while (f >> prime)
    {
        if (prime > N)
            break;
        primes.push_back(prime);
    }
    f.close();

    for (auto prime: primes)
    {
        int64 R_mod = 1;
        unordered_set<int64> seen;
        seen.insert(1);
        bool is_divisible = false;

        vector<int64> pp10m = vector<int64>(prime+1, 1);
        pp10m[1] = 10 % prime;
        for (int64 i = 1; i <= prime; i++)
            for (int64 j = 0; j < 10; j++)
                pp10m[i] = (pp10m[i] * pp10m[i-1]) % prime;
        for (int64 i = 1; i < prime; i++)
        {
            int64 prev_R_mod = R_mod;
            R_mod = 0;
            for (int64 j = 0; j < 10; j++)
            {
                R_mod = (R_mod * pp10m[i]) % prime;
                R_mod = (R_mod + prev_R_mod) % prime;
            }
            if (R_mod == 0)
            {
                is_divisible = true;
                break;
            }
            else if (seen.find(R_mod) != seen.end())
                break;
            seen.insert(R_mod);
        }
        if (!is_divisible)
           ans = ans + prime;
    }
    cout << ans << endl;

    return 0;
}