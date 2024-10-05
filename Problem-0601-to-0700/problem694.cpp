// Question: https://projecteuler.net/problem=694

// For each cube-full number c, it is one of the divisor of N // c numbers in range [1, N].
// So, the answer of this problem is sum_{cube_full c <= N} (N // c)

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

constexpr ui pow10(ui exp)
{
    if (exp == 0)
        return 1;
    return 10 * pow10(exp-1);
};

const ui N = pow10(18);

string ui128_to_string(__uint128_t n)
{
    stringstream stream;
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __uint128_t& n)
{
    os << ui128_to_string(n);
    return os;
}

vector<ui> get_primes_1e6()
{
    vector<ui> primes;

    ifstream input_stream("../inputs/primes_1e6.txt");
    unsigned int n;
    while (input_stream >> n)
    {
        if (n > N)
            break;
        primes.push_back(n);
    }
    input_stream.close();

    return primes;
}

/*
ui dfs(ui depth, const vector<ui>& primes, const ui& max_depth,
       ui curr_prod, const ui& max_prod, const ui& N)
{
    if (curr_prod > max_prod)
        return 0;

    if (depth >= max_depth)
        return 0;

    ui count = 0;

    ui base = primes[depth];

    ui next_prod = curr_prod;
    count += dfs(depth+1, primes, max_depth, next_prod, max_prod, N);

    next_prod = curr_prod * (base * base * base);
    while (next_prod <= max_prod)
    {
        count += N / next_prod;
        count += dfs(depth+1, primes, max_depth, next_prod, max_prod, N);
        next_prod *= base;
    }

    return count;
}
*/

ui dfs(ui prime_index, const vector<ui>& primes, const ui& max_prime_index,
       ui curr_prod, const ui& max_prod, const ui& N)
{
    if (curr_prod > max_prod)
        return 0;

    ui count = 0;

    ui base = primes[prime_index];

    ui next_prod = curr_prod * (base * base * base);
    while (next_prod <= max_prod)
    {
        count += N / next_prod;

        for (ui next_prime_index = prime_index+1;
                next_prime_index < max_prime_index;
                next_prime_index++)
        {
            ui next_count = dfs(next_prime_index, primes, max_prime_index,
                                next_prod, max_prod, N);
            if (next_count == 0) // next_prod must have been bigger than max_prod
                break;
            count += next_count;
        }

        next_prod *= base;
    }

    return count;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes_1e6();
    ui n_primes = primes.size();

    for (ui first_index = 0; first_index < n_primes; first_index++)
        ans += dfs(first_index, primes, n_primes, 1, N, N);

    ans += N;

    cout << ans << endl;

    return 0;
}
