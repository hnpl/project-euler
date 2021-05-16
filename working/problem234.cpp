// g++ problem234.cpp -std=c++17

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

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

const ui N = 999966663333ULL;
const ui M = ui(sqrt(N)+100);

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

inline ui square(const ui& x)
{
    return x*x;
}

inline ui partial_sum_multiple(const ui& a, const ui& b, const ui& m) // sum of all integers k in [a,b] such that k is divisible by m
{
    ui lowerbound = (a + m - 1) / m * m;
    ui upperbound = b / m * m;
    if (upperbound == 0 || lowerbound > upperbound)
        return 0;
    ui count = (upperbound - lowerbound) / m + 1;
    return lowerbound * count + (count * (count - 1) / 2) * m;
}

int main()
{
    ui ans = 0;

    auto primes = get_primes(M);

    auto prev_it = primes.cbegin();
    auto curr_it = next(primes.cbegin());
    while (true)
    {
        ui left_prime = *prev_it;
        ui right_prime = *curr_it;
        ui lowest_common_multiple = ui(left_prime) * right_prime;
        ui lowerbound = square(left_prime) + 1;
        ui upperbound = min(square(right_prime) - 1, N);

        if (lowerbound > N)
            break;
        
        ans += partial_sum_multiple(lowerbound, upperbound, left_prime);
        ans += partial_sum_multiple(lowerbound, upperbound, right_prime);
        ans -= 2 * partial_sum_multiple(lowerbound, upperbound, lowest_common_multiple);

        prev_it++;
        curr_it++;
    };

    cout << ans << endl;
    return 0;
}