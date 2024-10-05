// Question: https://projecteuler.net/problem=719

// Brute force all possible combinations.

#include<iostream>
#include<string>

using namespace std;

#define endl "\n"

typedef int64_t i64;

const i64 N = 1000000;

bool DFS(i64 n, i64 length, i64 sum, i64 target)
{
    if (length <= 1)
        return n + sum == target;
    else if (n + sum == target)
        return true;

    bool found = false;
    i64 mask = 1;

    i64 left_n = n;
    i64 right_n = 0;

    for (i64 i = 1; i <= length && !found; i++)
    {
        i64 r = left_n % 10;
        left_n = left_n / 10;
        right_n = right_n + mask * r;
        if (right_n > target)
            break;
        found = found || DFS(left_n, length - i, sum + right_n, target);
        mask = mask * 10;
    }
    return found;
}

int main()
{
    i64 ans = 0;
    for (i64 n = 2; n <= N; n++)
        if (DFS(n*n, to_string(n*n).length(), 0, n))
            ans = ans + n*n;
    cout << ans << endl;
    return 0;
}