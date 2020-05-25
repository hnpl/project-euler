// Question: https://projecteuler.net/problem=129

/*
    Lemma 1: A(n) <= n
    We have that, for all k such that GCD(n,10)=1, there exists k such that n|R(k).
    Assume that A(n) > n and GCD(n,10)=1.
    We have that modulus are periodic (don't know maths term for this).
    Since we have n different modulus for modulo of n, the length of the cycle must be <=n (and has <=n different values).
    Since the cycle has the length of <= n, and since there exists k such that n|R(k), A(n) = k < n (contradiction).
    So A(n) <= m.
    QED.

    So, we can start searching from 1000000.
*/

#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

typedef int64_t int64;

const int64 N = 1000000;

int64 A(int64 n)
{
    int64 k = 1;
    int64 r = 1;
    while (!(r == 0))
    {
        r = (10*r + 1) % n;
        k = k + 1;
    }
    return k;
}

int main()
{
    int64 ans = 0;

    for (int64 i = N; ; i++)
    {
        if (i % 2 == 0 || i%5 == 0)
            continue;
        if (A(i) > N)
        {
            ans = i;
            break;
        }
    }
    cout << ans << "\n";
    
    return 0;
}
