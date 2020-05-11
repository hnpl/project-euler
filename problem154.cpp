// Question: https://projecteuler.net/problem=154

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
            //i64 n_equals = (i==j) + (j==k) + (k==i);
            if (n10 >= 12)
            {
                //if (n_equals == 3) // not occur
                //    ans += 1;
                //else if (n_equals == 1)
                //    ans += 3;
                //else
                //    ans += 6;
                ans += 1;
            }
        }
    }
    cout << ans << "\n";
    return 0;
}