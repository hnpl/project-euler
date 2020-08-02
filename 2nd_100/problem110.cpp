// Problem: https://projecteuler.net/problem=110

/*
    Same reasoning as problem 108.
    However, since N is large, it is now infeasible to search all integers to find the smallest n such that n^2 has N divisors.
    This time, we build a smallest n^2 having N divisors.
    
    Observation 1:
        Let an arbitrary positive integer n = (p_1 ^ a_1) * (p_2 ^ a_2) * ... * (p_m ^ a_m),
            where p_i are primes for all i, and a_i are positive integers.
        The number of divisors of n is (1 + a_1) * (1 + a_2) * ... * (1 + a_m).
        Why?
            This is the same problem as the following problem:
                Suppose we have m boxes, and box 1 has (a_1 + 1) balls (representing p_1^0, p_1^1, ... , p_1^a_1)
                                             box 2 has (a_2 + 1) balls (representing p_2^0, p_2^1, ... , p_2^a_2)
                                             ...
                                             box m has (a_m + 1) balls (representing p_m^0, p_m^1, ... , p_m^a_m)
                Find the number of ways of selecting m balls (1 ball per box).
            The answer to the above problem is the same as the number of divisors of n.
        Hence, the number of divisors of n^2 is (1 + 2*a_1) * (1 + 2*a_2) * ... * (1 + 2*a_m).
    
    Observation 2:
        We need at most 14 prime factors (ceil(log3(4000000)) = 14)
            (since (1 + 2*1) ^ k where k = 14 is the first number > 4000000 among all choices of k).
        The 14th prime is 43.

    Observation 3:
        For increasing sequence p_1, p_2, ..., p_m; a_1 >= a_2 >= ... >= p_m
    
    So, we want to minimize (p_1 ^ a_1) * (p_2 ^ a_2) * ... * (p_m ^ a_m)
    with the constraint     (1 + a_1) * (1 + a_2) * ... * (1 + a_m) >= 2*N-1
    and we know p_1, p_2, ..., p_m, as well as m <= 14.
*/

#include<iostream>
#include<vector>
#include<queue>
#include<cmath>
#include<string>
#include<sstream>
#include<algorithm>
#include<cassert>

using namespace std;

#define endl "\n"

typedef __uint128_t ui;

const ui N = 4000000;
const ui MAX_PRIME = 43;
constexpr ui pow2(ui exponent)
{
    ui ans = 1;
    ui coeff = 2;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            ans = ans * coeff;
        exponent = exponent / 2;
        coeff = coeff * coeff;
    }
    return ans;
};

const ui MAX_n = pow2(96);


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

ui pow(ui base, ui exponent)
{
    ui ans = 1;
    ui coeff = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            ans = ans * coeff;
        exponent = exponent / 2;
        coeff = coeff * coeff;
    }
    return ans;
}

ui DFS(ui depth, ui curr_n, ui curr_n_divisors, const vector<ui>& primes, ui target_n_divisors, ui prev_exponent)
{
    ui n = primes.size();
    ui base = primes[depth];

    ui ans = MAX_n;

    if (depth == n - 1)
    {
        ui exponent_lowerbound = ceil(target_n_divisors*1.0/curr_n_divisors - 1) / 2;
        while (curr_n_divisors * (1 + 2*exponent_lowerbound) < target_n_divisors)
            exponent_lowerbound = exponent_lowerbound + 1;
        ui exponent_upperbound = int(log((MAX_n*1.0/curr_n)) / log(1.0*base));
        ui exponent = exponent_lowerbound; 
        ui n_divisors = curr_n_divisors * (1 + 2 * exponent);
        if (exponent_upperbound < exponent_lowerbound)
            return MAX_n;
        return curr_n * pow(base, exponent);
    }
    else
    {
        ui exponent_lowerbound = prev_exponent;
        ui coeff = pow(base, exponent_lowerbound);
        for (ui exponent = exponent_lowerbound; curr_n*coeff < ans; exponent++)
        {
            ui next_n = curr_n * coeff;
            ui next_n_divisors = curr_n_divisors * (1 + 2*exponent);
            if (next_n_divisors >= target_n_divisors)
                return min(next_n, ans);
            if (next_n > ans)
                break;
            ans = min(ans, DFS(depth + 1, next_n, next_n_divisors, primes, target_n_divisors, exponent));
            coeff = coeff * base;
        }
    }
    return ans;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes(MAX_PRIME);
    reverse(primes.begin(), primes.end());
    
    ans = DFS(0, 1, 1, primes, 2*N - 1, 0);

    cout << ans << endl;

    return 0;
}
