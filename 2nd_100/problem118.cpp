// Problem: https://projecteuler.net/problem=118

/*
    - Since a number containing each of 1, 2, ... , 9 once is divisible by 3, we only need to check the primes of fewer than to 9 digits.
    - Since each set contains each digit at most once, we can encode the digits appearing in the set using bits.
        Let f(S) = bits[1..9] where bits[k] = 1 if digit k appears in S, and bits[k] = 0 otherwise.
    - Let g(n) indicate the number of digits of n.
    - For a list of primes P where g(P[a]) <= g(P[b]) for all a < b.
    - Let DP[a][b] indicate the number of such a set S as described in the problem statement where f(S) = b and S is built from P[i] for all i <= a.
        + Then we have, DP[a+1][bitwise-OR(b,f(P[a+1]))] = sum{(DP[a][b]) | for all b such that bitwise-AND(b,f(P[a+1])) = 0}.
*/

#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>
#include<unordered_set>
#include<bitset>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 100'000'000ULL;

constexpr ui pow2(ui n)
{
    if (n == 0)
        return 1;
    return 2 * pow2(n-1);
};

ui hash_freq(ui n)
{
    ui hash = 0;

    for (; n > 0; n /= 10)
    {
        ui digit = n % 10;
        bool digit_encountered = ((hash >> digit) & 0x1) == 1;
        if ((digit == 0) || digit_encountered)
        {
            hash = 1;
            return hash;
        }
        hash = hash | (1 << digit);
    }

    return hash;
}

vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(float(n)));
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
    ui ans = 0;

    vector<ui> primes = get_primes(N);
    vector<ui> filtered_primes_hashes;
    for (auto prime: primes)
    {
        ui prime_hash = hash_freq(prime);
        if ((prime_hash & 0x1) == 0)
            filtered_primes_hashes.push_back(prime_hash >> 1);
    }

    ui n = filtered_primes_hashes.size();
    ui max_hash = 0b111111111;

    vector<vector<ui>> DP = vector<vector<ui>>(2, vector<ui>(max_hash+1, 0));
    ui curr_row = 0;
    ui prev_row = 1;
    for (ui n_primes = 1; n_primes <= n; n_primes++)
    {
        curr_row = n_primes % 2;
        prev_row = 1 - curr_row;
        ui prime_hash = filtered_primes_hashes[n_primes-1];
        DP[curr_row] = DP[prev_row]; 
        DP[curr_row][prime_hash] += 1;
        for (ui hash = 0; hash <= max_hash; hash++)
        {
            if ((hash & prime_hash) == 0)
                DP[curr_row][hash | prime_hash] += DP[prev_row][hash];
        }
    }

    cout << DP[curr_row][0b111111111] << endl;

    return 0;
}
