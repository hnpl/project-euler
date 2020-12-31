// Question: https://projecteuler.net/problem=268

/*
    . Let P be the set of primes of less than 100
    . Using the inclusion-exclusion principle, we have,

        answer = AT_LEAST_1 - ONLY_1 - ONLY_2 - ONLY_3

     where,
        AT_LEAST_1: number of numbers having at least one prime factor in P
        ONLY_i : number of numbers having exactly i prime factors that are in P

    . Let f(a1, a2, ...) be a function that its value is the number of numbers divisible by a1, a2, ...
        So, f(a1, a2, ..., an) = N / (a1 * a2 * ... * an)
    . Let g(a1, a2, ...) be a function that its value is the number of numbers such that for each number K,
        the prime factors of less than 100 of K are exactly this set {a1, a2, ...}
        So, using the inclusion-exclusion principle, we have,
            g(a1, a2, ..., an) = sum(y*f(X) where X are all combinations of P where {a1, a2, ..., an} is a subset of P,
                                     and y = 1 if |X| has the same modulo 2 with n, and y = -1 otherwise)
        For example,
            g(a1) = f(a1) - f(a1, a2) - f(a1, a3) - ... - f(a1, an) + f(a1, a2, a3) + f(a1, a2, a4) + ...
            # Note that, for this case, 
            g(a1, a2) = f(a1, a2) + f(a1, a3) + ... + f(a1, an) - f(a1, a2, a3) - f(a1, a2, a4) - ...
    . Let h(n) be a function that its value is the number of numbers having exactly n prime factors of less than 100
      In other words, h(n) = sum(g(X) where X are all combinations of P where |X| = n)
      For this problem, we are interested in h(1), h(2), h(3).
    . nCk means n chooses k

    . Using the inclusion-exclusion principle, we have,
        AT_LEAST_1 =   sum(f(p), for all p in P)
                     - sum(f(p1, p2), for all (p1, p2) in P^2)
                     + sum(f(p1, p2, p3), for all (p1, p2, p3) in P^3)
                     - ...
                   =   sum(y*f(X) for all combinations X of P, where y = 1 if |X| mod 2 = 1, and y = -1 otherwise)
        
        ONLY_1 = h(1)
               = g(a1) + g(a2) + ... + g(an)
               =   f(a1) - f(a1, a2) - f(a1, a3) - ... - f(a1, an) + f(a1, a2, a3) + f(a1, a2, a4) + ...
                 + f(a2) - f(a2, a1) - f(a2, a3) - ... - f(a2, an) + f(a2, a1, a3) + f(a2, a1, a4) + ...
                 + ...
                 + f(an) - f(an, a1) - f(an, a3) - ... - f(an, a_{n-1}) + f(an, a1, a2) + f(an, a1, a3) + ...
               =   sum(1C1 * f(p) for all p in P)
                 - sum(2C1 * f(p1, p2) for all (p1, p2) in P)
                 + sum(3C1 * f(p1, p2, p3) for all (p1, p2, p3) in P) # why? for each (p1, p2, p3), we have f(p1, p2, p3) appears once in each of f(p1), f(p2), f(p3)
                 - ...

        Similarly,
        ONLY_2 = h(2)
               =   sum(2C2 * f(p1, p2) for all (p1, p2) in P)
                 - sum(3C2 * f(p1, p2, p3) for all (p1, p2, p3) in P) # why? for each (p1, p2, p3), we have f(p1, p2, p3) appears once in each of f(p1, p2), f(p1, p3), f(p2, p3)
                 + sum(4C2 * f(p1, p2, p3, p4) for all (p1, p2, p3, p4) in P)
                 - ...
        
        ONLY_3 = h(3)
               =   sum(3C3 * f(p1, p2, p3) for all (p1, p2, p3) in P)
                 - sum(4C3 * f(p1, p2, p3, p4) for all (p1, p2, p3, p4) in P)
                 + ...

    . 
    count f in h |             f(p1) |         f(p1, p2) |     f(p1, p2, p3) | f(p1, p2, p3, p4) | f(p1, p2,..., pn) |
    -------------+-------------------+-------------------+-------------------+-------------------+-------------------+
      AT_LEAST_1 |                 1 |                -1 |                 1 |                -1 |                 t | t = 1 if n is odd, t = -1 otherwise
            h(1) |               1C1 |              -2C1 |               3C1 |              -4C1 |           x * nC1 | x = 1 if n is odd, x = -1 otherwise
            h(2) |                 0 |               2C2 |              -3C2 |               4C2 |           y * nC2 | y = 1 if n is even, y = -1 otherwise
            h(3) |                 0 |                 0 |               3C3 |              -4C3 |           z * nC3 | z = 1 if n is odd, z = -1 otherwise
    
    . We have that nC1 - nC2 + nC3 = (n^3+11n)/6 - n^2
    . So, answer = sum[(t - t * ((n^3+11n)/6 - n^2)) for all combinations p of P where n = |P|, t = 1 if n is odd, t = -1 otherwise]
        
 */

#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>


using namespace std;

typedef __int128_t ui;
#define endl "\n"

constexpr ui pow10(ui n)
{
    if (n == 0)
        return 1;
    return pow10(n-1) * 10;
};

const ui N = pow10(16);

const vector<ui> primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
                        53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

// this function only prints positive numbers correctly
string to_string(__int128_t n)
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

ostream& operator<<(ostream& os, const __int128_t n)
{
    os << to_string(n);
    return os;
}

constexpr ui get_coeff(ui n)
{
    if ((n % 2) == 1)
        return -(n*n*n+11*n) / 6 + n*n + 1;
    else
        return (n*n*n+11*n) / 6 - n*n - 1;
};

void dfs(ui depth, ui n_on, ui curr_val, const ui& max_depth,
         const vector<ui>& primes, ui& ans)
{
    if (depth == max_depth)
        return;
    ui next_prime = primes[depth];
    ui next_val = curr_val * next_prime;

    dfs(depth+1, n_on, curr_val, max_depth, primes, ans);
    if (next_val < N)
    {
        ans += N / next_val * get_coeff(n_on+1);
        dfs(depth+1, n_on+1, next_val, max_depth, primes, ans);
    }
}

int main()
{
    ui ans = 0;
    dfs(0, 0, 1, primes.size(), primes, ans);
    cout << ans << endl;
    return 0;
}