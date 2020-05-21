// Question: https://projecteuler.net/problem=500

// Given that 'x' is the smallest number having 2^i divisors
// How to find the smallest number having 2^(i+1) divisors?

// n = 1                     -> smallest n having 2^0 divisor
// n = 1 * 2                 -> smallest n having 2^1 divisors
// n = 1 * 2 * 3             -> smallest n having 2^2 divisors
// n = 1 * 2 * 3 * 2^2       -> smallest n having 2^3 divisors
// n = 1 * 2 * 3 * 2^2 * 5   -> smallest n having 2^4 divisors

// -> to have 2 times more divisors, we multiply x with the smallest number p^i such that p^i > q^j for all (q, j) in prime_factors(x) and p is a prime, and i = 1 if p!=q for all (q, j) in prime_factors(x), otherwise, i = j+1 if p == q if (q, j) in prime_factors(x).

// why? assume n = p^i * c with p^i is a prime factor and c does not have p^i in its prime factors. 
// Assume c has x divisors. We have p^i has (i+1) divisors. So n has x(i+1) divisors.
// So, if we multiply n by p^(i+1), we have n * p^(i+1) = p^(2i+1) * c, which has x(2i+2) factors.

// In the algorithm above, i is always a power of 2.
// If the previous time we multiply n by a power of p (eg, p^(2^i)), then n has p^(2^(i+1)-1) in its prime factors, which means n has 2^(i+1) * c divisors.
// If we multiply n by p^(2^i) * p^(2^i) = p^(2^(i+1)), then n has p^(2^(i+2)-1) in its prime factors, which means n has 2^(i+2) * c divisors. So, we double the number of factors compared to n.

#include<iostream>
#include<unordered_map>
#include<queue>
#include<functional>
#include<fstream>
#include<vector>
#include<cmath>
#include<cstring>

using namespace std;

typedef uint64_t ui64;
typedef __uint128_t ui128;

const ui64 N = 500500;
const ui128 MOD = 500500507LL;
const ui64 SIEVE_SIZE = 10000000;

void sieve(ui64 size, vector<bool>& array)
{
    ui64 upperbound = (ui64)(sqrt(size));
    for (ui64 i = 2; i <= upperbound; i++)
    {
        if (!array[i])
            continue;
        for (ui64 j = i*2; j <= size; j+=i)
            array[j] = false;
    }
}

int main()
{
    ui128 ans = 1;

    priority_queue<ui128, vector<ui128>, greater<ui128>> q;
    unordered_map<ui128, ui128> base_prime_map; // this map is the same as f(x^y) = x

    vector<bool> is_prime = vector<bool>(SIEVE_SIZE+1, true);
    sieve(SIEVE_SIZE, is_prime);

    for (ui64 i = 2; i <= SIEVE_SIZE; i++)
        if (is_prime[i])
        {
            q.push(i);
            base_prime_map[i] = i;
        }

    for (ui64 i = 1; i <= N; i++)
    {
        auto p = q.top();
        q.pop();
        ans = (ans * p) % MOD;
        ui128 new_factor = p * p;
        q.push(new_factor);
    }

    cout << (ui64)(ans) << "\n";

    return 0;
}
