// Question: https://projecteuler.net/problem=512

/*
 * Ideas,
 *   - We have that, for a pair of positive integers n and k,
 *         euler_phi(n) = n * prod_{prime p divides n} (1 - 1/p)
 *     and euler_phi(n^k) = n^k * prod_{prime p divides n} (1 - 1/p) = n^(k-1) * euler_phi(n)
 *   - Therefore, euler_phi(n^k) mod (n+1) = [n^(k-1) mod (n+1)] * [euler_phi(n) mod(n+1)]
 *                                         = (-1)^(k-1) * euler_phi(n) (note that all moduli were lifted as euler_phi(n) < n and (-1)^(k-1) is either 1 or -1
 *   - Note that, f(n) = sum_{k=1..n} euler_phi(n^k) mod (n+1)
 *                     = sum_{k=1..n} [(-1)^(k-1) * euler_phi(n)] mod (n+1)
 *                     = euler_phi(n) * sum_{k=1..n} [(-1)^(k-1)] mod (n+1)
 *                        _
 *                       |  euler_phi(n) if n is odd
 *                     = |            0  if n is even
 *                       |_
 *                       (this is because sum_{k=1..n} [(-1)^(k-1)] is an alternating function and the terms will all be canceled out if the number of terms is even.
 *   - So, g(n) = sum_{1 <= i <= N where i is odd} euler_phi(i).
 *   - In this implementation, we implemented a naive sieve to compute euler_phi values for all odd i <= N.
 *   - There is a faster way to do euler_phi summation using Mertens functions, but it is not implemented here.
 */

#include<algorithm>
#include<cassert>
#include<cmath>
#include<iostream>
#include<numeric>
#include<vector>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return pow(base, exp-1) * base;
};

const ui N = pow(10, 8) * 5;

vector<ui> get_euler_phi(const ui& n)
{
    vector<ui> euler_phi = vector<ui>(n+1, 0);
    iota(euler_phi.begin(), euler_phi.end(), 0);
    euler_phi[2] = 1;
    for (ui i = 3; i <= n; i+=2)
    {
        if ((i-1) % 100000 == 0)

            cout<< i << endl;
        if (euler_phi[i] != i) // if this is true, this is not a prime number
            continue;
        for (ui p = i; p <= n; p += 2*i)
        {
            euler_phi[p] /= i;
            euler_phi[p] *= i-1;
        }
    }

    return euler_phi;
}

int main()
{
    ui ans = 0;
    auto euler_phi = get_euler_phi(N);
    for (ui i = 1; i <= N; i+=2)
        ans += euler_phi[i];
    cout << ans << endl;
    return 0;
}

