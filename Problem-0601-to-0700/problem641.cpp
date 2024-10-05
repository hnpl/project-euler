// Question: https://projecteuler.net/problem=641

/**
 * . Consider the numbers to be the array `arr`. Initially, arr[k] = 1 for all k.
 * . After the process, the number in arr[k] is (the number of divisors of k except 1) mod 6.
 * . Let k be an integer having prime factors of k = (p_1)^(exp_1) * (p_2)^(exp_2) * ... * (p_m)^(exp_m)
 * . The number of divisors of k (including the divisor 1) is,
 *      product_{i = 1..m} (exp_i + 1)
 * . So, arr[k] = 1 + [product_{i = 1..m} (exp_i + 1)-1] (mod 6)
 *               ^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *           initial       number of divisors except 1
 *   So, arr[k] = [product_{i = 1..m} (exp_i + 1)] (mod 6)
 * . We want to find the number of arr[k] where arr[k] = 1.
 * .             arr[k] = 1
 *   then        product_{i = 1..m} (exp_i + 1) (mod 6) = 1
 *   
 *   then        product_{i = 1..m} (exp_i + 1) (mod 2) = 1
 *         and   product_{i = 1..m} (exp_i + 1) (mod 3) = {1,2} where there are an even number of i where (exp_i + 1) (mod 3) = 2
 *
 *   then        for all i, (exp_i + 1) (mod 2) = 1
 *                   and    (exp_i + 1) (mod 3) = {1, 2} where there are an even number of i where (exp_i + 1) (mod 3) = 2
 *
 *   then        for all i, exp_i (mod 2) = 0 and exp_i (mod 3) = {0, 1} where there are an even number of i where (exp_i) (mod 3) = 1.
 *
 * . We can use dfs to construct all possible numbers <= N where, for each number n = product_{i = 1..m} (exp_i + 1), the number n satisfies this condition: for all i, exp_i (mod 2) = 0 and exp_i (mod 3) = {0, 1} where there are an even number of i where (exp_i) (mod 3) = 1.
 * . Note that the first number n such that n (mod 2) = 0 and n (mod 3) \in {0, 1} is 4. So, we only to find primes upto 10^(36/4) = 10^9.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cmath>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

constexpr ui pow10(ui n)
{
    if (n == 0)
        return 1;
    return 10 * pow10(n-1);
};

const ui N = pow10(36);
const double NF = 1e36;

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


constexpr __uint128_t sqrt(const __uint128_t& n)
{
    __uint128_t l = 1;
    __uint128_t r = 1ULL << 63;
    __uint128_t m = (l+r)/2;
    __uint128_t ms = m*m;

    while (l < r)
    {
        m = (l+r)/2;
        ms = m*m;
        if (ms > n)
            r = m;
        else
            l = m+1;
    }

    if (ms > n)
        m--;

    return m;
};

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

ui dfs(ui prime_index, const vector<ui>& primes, const ui& n_primes,
       ui curr_prod, double curr_prod_f, const ui& max_prod, ui n_r1_mod3)
{
    if (prime_index >= n_primes)
        return 0;

    if (curr_prod_f > max_prod)
        return 0;

    ui count = 0;

    ui base = primes[prime_index];
    double basef = double(base);
    ui next_prod = curr_prod * base * base * base * base;
    double next_prod_f = curr_prod_f * pow(basef, 4);
    ui next_n_r1_mod3 = n_r1_mod3;
    for (ui exp = 4; next_prod_f <= NF; exp += 2)
    {
        if (prime_index == 0)
            cout << exp << endl;

        if (exp % 3 == 1)
            next_n_r1_mod3 = n_r1_mod3 + 1;
        else
            next_n_r1_mod3 = n_r1_mod3;

        if (!(exp % 3 == 2))
        {
            if (next_n_r1_mod3 % 2 == 0)
                count += 1;
            for (ui next_prime_index = prime_index + 1; next_prime_index < n_primes; next_prime_index++)
            {
                ui next_count = dfs(next_prime_index, primes, n_primes, next_prod, next_prod_f, max_prod, next_n_r1_mod3);
                if (next_count == 0)
                    break;
                count += next_count;
            }
        }

        next_prod = next_prod * base * base;
        next_prod_f = next_prod_f * basef * basef;
    }

    return count;
}

int main()
{
    ui ans = 0;

    auto primes = get_primes(pow10(9));

    ui n_primes = primes.size();
   
    for (ui first_index = 0; first_index < n_primes; first_index++)
        ans += dfs(first_index, primes, n_primes, 1, 1.0, N, 0);

    ans += 1; // arr[1] has never changed

    cout << ans << endl;

    return 0;
}
