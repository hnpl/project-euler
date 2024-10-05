// Question: https://projecteuler.net/problem=820

/*
 * How to solve:
 * . We can find the n^th digit of 1/k via integer arithmetic.
 *   . nth_digit(1/k) = floor((10**n) / k) (mod 10)
 *   . Suppose alpha = floor((10**n) / k) (mod 10)
 *    -> 10 * c + alpha <= (10**n) / k < 10 * c + alpha + 1
 *    i.e., c is the largest integer such that 10 * c + alpha <= (10**n) / k    [clause 1]
 *    Also, 10 * c + alpha <= (10**n) / k
 *       -> k * 10 * c + k * alpha <= (10**n)
 *       ->              k * alpha = (10**n) mod (10*k) (this is due to [clause 1])
 *       ->                  alpha = [(10**n) mod (10*k)] / k
 */

#include <iostream>
#include <cstdint>

using namespace std;

typedef uint64_t ui;

const ui N = 10000000;

ui pow(ui base, ui exp, ui mod)
{
    ui ans = 1;

    while (exp > 0)
    {
        if ((exp % 2) == 1)
        {
            ans *= base;
            ans %= mod;
        }
        base *= base;
        base %= mod;
        exp /= 2;
    }

    return ans;
}

int main()
{
    ui ans = 0;
    for (ui k = 1; k <= N; k++)
        ans += pow(10, N, k*10) / k;
    cout << ans << endl;
    return 0;
}
