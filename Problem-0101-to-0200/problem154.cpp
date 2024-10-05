// Question: https://projecteuler.net/problem=154

/*
    . The coefficients of trinomial expansion follows multinomial theorem.
    . The coefficients of (x+y+z)^N are in the form of multinomial(i, j, N-i-j) where i >= 0, j >= 0, N-i-j > =0.
    . We have multinomial(i, j, N-i-j) = N! / i! / j! / (N-i-j)!.
    . We want to find how many coefficients of the expansion are multiples of 10^12.
    . Note that, for a number k, we can find the number of trailing digit 0's as follows,
        Let p be the largest possible integer such that (2^p) | k. (p is called the order of the prime factor 2)
        Let q be the largest possible integer such that (5^q) | k. (q is called the order of the prime factor 5)
        The number of trailing zeros of k is min(p, q).
    . Let m2[1..N] denote the order of the prime factor 2 of numbers in the range [1..N].
        We have that, for every 2 number, there is one number that is divisible by 2.
                      for every 2^2 number, there is one number that is divisible by 2^2.
                      for every 2^3 number, there is one number that is divisible by 2^3.
                      and so on
        So, for each power of 2, namely 2^p, we can iterate through each multiple of 2^p, namely k*(2^p) and increase m2[k*(2^p)] by 1.
        For a number that has the order p for prime factor 2, the number will be visited p times.
        Therefore, m2[k] denote the order of the prime factor 2 of k.
    . Let m2f[k] denote the order of the prime factor 2 of factorial k!.
      We have that, k! = k * ((k-1)!).
      So, m2f[k] = m2[k] + m2f[k-1].
    . We can follow a similar procedure to find m5f.
    . The number of trailing zeros of k! is m10f = min(m2f[k], m5f[k]).
    . So, we can find the number of trailing zeros of multinomial(i, j, N-i-j) for all i+j <= N from the precalculated numbers.
      The number of trailing zeros of multinomial(i, j, N-i-j) is min(m2f[N] - m2f[i] -m2f[j] - m2f[N-i-j], m5f[N] - m5f[i] -m5f[j] -m5f[N-i-j]).
*/
#include<iostream>
#include<cstring>

typedef int64_t i64;

using namespace std;

int main()
{
    const i64 N = 200000;
    i64 m2[N+1];
    i64 m5[N+1];
    i64 m2f[N+1];
    i64 m5f[N+1];
    
    memset(m2, 0, sizeof(m2));
    memset(m5, 0, sizeof(m5));

    i64 x = 2;
    while (x <= N)
    {
        for (i64 i = x; i <= N; i += x)
            m2[i] += 1;
        x = x * 2;
    }

    m2f[0] = 0;
    for (i64 i = 1; i <= N; i++)
        m2f[i] = m2f[i-1] + m2[i];

    x = 5;
    while (x <= N)
    {
        for (i64 i = x; i <= N; i += x)
            m5[i] += 1;
        x = x * 5;
    }

    m5f[0] = 0;
    for (i64 i = 1; i <= N; i++)
        m5f[i] = m5f[i-1] + m5[i];

    i64 ans = 0;

    for (i64 i = 0; i <= N; i++)
    {
        for (i64 j = 0; j <= N; j++)
        {
            i64 k = N - i - j;
            if (k < 0)
                break;
            i64 n2 = m2f[N] - m2f[i] - m2f[j] - m2f[k];
            i64 n5 = m5f[N] - m5f[i] - m5f[j] - m5f[k];
            i64 n10 = min(n2, n5);
            if (n10 >= 12)
                ans += 1;
        }
    }
    cout << ans << "\n";
    return 0;
}
