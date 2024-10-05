// Question: https://projecteuler.net/problem=142

#include<iostream>
#include<vector>
#include<unordered_set>

using namespace std;

typedef int64_t i64;

const i64 N = 10000;

#define endl "\n"

bool check(i64 x, i64 y, i64 z, const unordered_set<i64>& sq_set)
{
    if (sq_set.find(y-z) != sq_set.end() &&
        sq_set.find(x+z) != sq_set.end() &&
        sq_set.find(x-z) != sq_set.end())
        return true;
    return false;
}

int main()
{
    vector<i64> sq;
    unordered_set<i64> sq_set;

    for (i64 i=1; i<=N; i++)
    {
        sq.push_back(i*i);
        sq_set.insert(i*i);
    }

    i64 ans = 0;
    bool stop = false;

    for (i64 i = 2; i < sq.size() && !stop; i++)
    {
        i64 a = sq[i];
        for (i64 j=1-a%2; j < i && !stop; j+=2)
        {
            i64 b = sq[j];
            i64 x = (a+b)/2;
            i64 y = (a-b)/2;
            for (i64 k = 0; k < i && !stop; k++)
            {
                i64 c = sq[k];
                i64 z = c - y;
                if (z < 0)
                    continue;
                if (z >= y)
                    break;
                if (check(x, y, z, sq_set))
                {
                    stop = true;
                    ans = x + y + z;
                }
            }
        }
    }

    cout << ans << endl;

    return 0;
}