// https://oeis.org/A064775 ; card = cardinality

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow10(ui n)
{
    if (n == 0)
        return 1;
    return 10 * pow10(n-1);
};

const ui N = pow10(10);
const ui sqrt_N = ui(sqrt(float(N)));

vector<ui> get_known_primes(string filename)
{
    vector<ui> primes;
    ifstream input_stream(filename);
    ui p;
    while (input_stream >> p)
        primes.push_back(p);
    input_stream.close();
    return primes;
}

vector<ui> get_primes_range(ui lowerbound, ui upperbound, const vector<ui>& known_primes)
{
    if (lowerbound % 2 == 0)
        lowerbound += 1;

    if (upperbound % 2 == 0)
        upperbound -= 1;

    ui offset = lowerbound;
    ui n = upperbound - lowerbound + 1;
    ui sqrt_upperbound = ui(sqrt(float(upperbound)));

    vector<ui> primes;
    primes.reserve(n/10);

    if (lowerbound > upperbound)
        return primes;

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    auto prime_it = known_primes.begin();
    prime_it++;
    for (; prime_it != known_primes.end(); prime_it++)
    {
        ui prime = *prime_it;
        if (prime > sqrt_upperbound)
            break;
        ui first_mod_prime_equal_zero = ((offset + prime - 1) / prime) * prime;
        if (first_mod_prime_equal_zero % 2 == 0)
            first_mod_prime_equal_zero += prime;
        for (ui np = first_mod_prime_equal_zero; np <= upperbound; np += 2*prime) // np -> multiple of p
            is_prime[(np-offset)/2] = false;
    }
    ui size = is_prime.size();
    for (ui i = 0; i < size; i++)
    {
        if (is_prime[i])
            primes.push_back(offset + i*2);
    }
    return primes;
}

int main()
{
    ui ans = 0;

    vector<ui> known_primes = get_known_primes("../inputs/primes_1e6.txt");

    // main calculation
    ans = N;
    for (auto prime: known_primes)
    {
        if (prime > sqrt_N)
            break;
        ans -= (prime - 1);
        ans -= 1; // the original recursion counts "p | n then p^2 <= n when p is prime"; while we want "p | n then p^2 < n when p is prime"
    }
    // we need to know primes ranging [2, 10**10]
    // we will split the range into 100 parts and perform sieve-ing on each of the parts
    //ui step = N / 100;
    ui step = N / 100;
    for (ui range_begin = sqrt_N+1; range_begin < N; range_begin += step)
    {
        ui range_end = min(range_begin + step - 1, N);
        auto primes = get_primes_range(range_begin, range_end, known_primes);
        for (auto prime: primes)
            ans -= N / prime;
    }
    cout << ans << endl;

    return 0;
}