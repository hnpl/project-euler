// Question: https://projecteuler.net/problem=204

// Brute-force: construct all numbers satifying the constraints.

#include<iostream>
#include<unordered_set>
#include<vector>
#include<cstring>
#include<cmath>

using namespace std;

typedef uint64_t ui;

#define endl "\n";

const ui N = 1000000000LL;
const ui M = 100;

ui DFS(vector<ui>& primes, ui current_index, ui base_product, ui N)
{
    ui total = 0;
    ui current_prime = primes.at(current_index);
    ui power_upperbound = ui(log(N/base_product) / log(current_prime));
    for (ui power = 1; power <= power_upperbound; power++)
    {
        base_product = base_product * current_prime;
        if (base_product > N)
            break;
        total = total + 1; // count base_product itself, next DFS does not count base_product
        for (ui next_prime_index = current_index + 1; next_prime_index < primes.size(); next_prime_index++)
        {
            ui next_prime = primes.at(next_prime_index);
            if (next_prime * base_product > N)
                break;
            total = total + DFS(primes, next_prime_index, base_product, N);
        }
    }
    return total;
}

int main()
{
    bool is_prime[M+1];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;

    for (ui i = 2; i <= M; i++)
    {
        if (!is_prime[i])
            continue;
        for (ui j = 2*i; j <= M; j+=i)
            is_prime[j] = false;
    }

    vector<ui> primes;
    primes.reserve(100);
    for (ui i = 2; i <= M; i++)
        if (is_prime[i])
            primes.push_back(i);
    ui ans = 0;
    for (ui i = 0; i < primes.size(); i++)
        ans = ans + DFS(primes, i, 1, N);
    cout << ans + 1 << endl;
    //           ^^^
    //        number '1' is not counted by DFS

    return 0;
}
