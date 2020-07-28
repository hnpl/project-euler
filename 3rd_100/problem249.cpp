// Question: https://projecteuler.net/problem=249

/*
    First, generate all primes from 1 to 5000. Let S = {prime | prime < 5000}.
    The maximum sum of a subset is 1548136.
    Let DP[i][j] is the number of ways of writing sum j using the first i primes in S.
    Then,
	DP[i][j+prime] = DP[i-1][j+prime] + DP[i-1][j]
                         ^^^^^^^^^^^^^^^^   ^^^^^^^^^^
                         choose not to      choose to use
			 use i^th prime     i^th prime
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 5000;

const ui LAST_DIGITS_MOD = 10'000'000'000'000'000ULL;

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

int main()
{
    auto primesN = get_primes(N);

    ui sum = 0;

    for (auto& prime: primesN)
	sum = sum + prime;

    auto primes = get_primes(sum);

    ui ans = 0;

    ui* DP[2];
    DP[0] = new ui[sum+1];
    DP[1] = new ui[sum+1];

    for (ui i = 0; i <= sum; i++)
	DP[0][i] = 0;

    for (ui i = 0; i <= sum; i++)
	DP[1][i] = 0;

    ui prev_i = 1;
    ui curr_i = 0;
    DP[0][0] = 1;
    
    for (ui i = 0; i < primesN.size(); i++)
    {
	ui prime = primesN[i];
	curr_i = (i+1) % 2;
	prev_i = 1 - curr_i;
	for (ui j = 0; j <= sum; j++)
	    DP[curr_i][j] = DP[prev_i][j];
	for (ui j = 0; j <= sum - prime; j++)
	    DP[curr_i][j+prime] = (DP[curr_i][j+prime] + DP[prev_i][j]) % LAST_DIGITS_MOD;
    }

    for (auto& prime: primes)
	ans = (ans + DP[curr_i][prime]) % LAST_DIGITS_MOD;

    cout << ans << endl;

    delete[] DP[0];
    delete[] DP[1];

    return 0;
}
