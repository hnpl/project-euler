// Question: https://projecteuler.net/problem=417
// g++ problem417_pari.cpp -I{$HOME}/pari/include/ -L{$HOME}/pari/lib -lpari

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>


using namespace std;

#define endl "\n"

typedef int64_t i64;

const long N = 100000000;

inline long find_cycle_length(long n)
{
    static i64 count = -1;
    count = count + 1;
    if (count % 1000000 == 0)
        pari_init(100000000,2);
    long ans = gtolong(order(gmodulss(10, n)));
    if (count % 1000000 == 1000000-1)
        pari_close();
    return ans;
}

int main()
{
    
    vector<long> lengths = vector<long>(N+1, -1);

    vector<long> non_coprime_2_5;

    for (long _2p = 1; _2p <= N; _2p*=2)
        for (long _2p5q = _2p; _2p5q <= N; _2p5q *= 5)
            non_coprime_2_5.push_back(_2p5q);

    for (auto n: non_coprime_2_5)
        lengths[n] = 0;

    std::sort(non_coprime_2_5.begin(), non_coprime_2_5.end());

    for (long n = 3; n <= N; n++)
    {
        if (lengths[n] >= 0)
            continue;
	long l = find_cycle_length(n);
        lengths[n] = l;
        for (auto non_coprime: non_coprime_2_5)
        {
            if (n * non_coprime <= N)
                lengths[n * non_coprime] = l;
            else
                break;
        }
    }

    auto ans = accumulate(lengths.begin() + 3, lengths.end(), static_cast<i64>(0));

    cout << ans << endl;


    return 0;   
}
