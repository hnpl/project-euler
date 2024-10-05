// Question: https://projecteuler.net/problem=618

/*
 * - Strategy:
 *      . Considering calculating S(N), i.e. sum of all numbers having N as their sum of prime factors.
 *          . Note that 0 < N < 50000, so we can build an array Prod, where Prod[K] is the sum of all numbers whose K as their sum of prime factors.
 *          . We can build Prod[K] by adding each of possible prime factors to Prod[K], i.e., suppose we are adding prime P and its powers,
 *              next_Prod[K] = sum_{exp=1 to ln(N)/ln(P)}(P**exp * prev_Prod[k - exp*P]) for all K.
 */

#include<vector>
#include<cmath>
#include<iostream>
#include<algorithm>
#include<cassert>
#include<unordered_set>

using namespace std;

typedef uint64_t ui;

const ui N = 24;
const ui MOD = 1'000'000'000;

vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(n));
    for (ui p = 3; p <= sqrt_n; p+=2)
    {
        if (!is_prime[(p-3)/2])
            continue;
        for (ui np = 3*p; np <= n; np += 2*p)
            is_prime[(np-3)/2] = false;
    }
    primes.push_back(2);
    ui size = is_prime.size();
    for (ui p = 0; 2*p+3 <= n; p++)
    {
        if (is_prime[p])
            primes.push_back(2*p+3);
    }
    return primes;
}

ui S(ui n, const vector<ui>& primes)
{
    ui ans = 0;

    ui n_primes = 0;
    for (; n_primes < primes.size(); n_primes++)
        if (primes[n_primes+1] > n)
            break;

    vector<ui> curr_prod = vector<ui>(n+1, 0);
    vector<ui> prev_prod = vector<ui>(n+1, 0);
    vector<ui> temp_prod = vector<ui>(n+1, 0);
    curr_prod[0] = 1;
    for (int i = n_primes; i >= 0; i--)
    {
        ui prime_to_add = primes[i];
        ui factor_to_add = prime_to_add;
        ui exp = 1;
        prev_prod = curr_prod;

        fill(temp_prod.begin(), temp_prod.end(), 0);
        //cout << "prime=" << prime_to_add << endl;
        while (prime_to_add * exp <= n)
        {
            fill(curr_prod.begin(), curr_prod.end(), 0);
            for (int prev = 0; prev <= n-exp*prime_to_add; prev++)
            {
                int curr = prev + exp*prime_to_add;
                curr_prod[curr] += prev_prod[prev] * factor_to_add;
                curr_prod[curr] %= MOD;
            }
            //cout << "exp=" << exp << " " << curr_prod[n] << endl;
            exp += 1;
            factor_to_add *= prime_to_add;
            factor_to_add %= MOD;
            for (ui j = 0; j <= n; j++)
            {
                temp_prod[j] += curr_prod[j];
                temp_prod[j] %= MOD;
            }
        }

        for (ui j = 0; j <= n; j++)
        {
            curr_prod[j] = (prev_prod[j] + temp_prod[j]) % MOD;
            //cout << curr_prod[j] << " ";
        }
        //cout << endl;
    }
    ans = curr_prod[n];

    return ans;
}

int main()
{
    vector<ui> F(N+1, 0);
    F[1] = 1;
    F[2] = 1;
    for (ui i = 3; i <= N; i++)
        F[i] = F[i-1] + F[i-2];
    auto primes = get_primes(F[N]+100);
    ui ans = 0;
    for (ui i = 2; i <= N; i++)
    {
        ui SF = S(F[i], primes);
        ans += SF;
        ans %= MOD;
        cout <<"F="<< F[i] << " " << SF << endl;
    }
    cout << ans << endl;
}
