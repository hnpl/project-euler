// Problem: https://projecteuler.net/problem=111

#include<iostream>
#include<vector>
#include<queue>
#include<cmath>
#include<string>
#include<sstream>
#include<algorithm>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 10;

constexpr ui p10(ui n)
{
    if (n==0)
        return 1;
    return 10 * p10(n-1);
};

const ui SQRT_MAX = ui(sqrt(p10(N)));
const vector<ui> pow10 = {p10(0), p10(1), p10(2), p10(3), p10(4), p10(5), p10(6), p10(7), p10(8), p10(9), p10(10)};

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

bool is_prime(ui n, const vector<ui>& primes)
{
    ui sqrt_n = sqrt(n);
    for (auto prime: primes)
    {
        if (prime > sqrt_n)
            return true;
        if (n % prime == 0)
            return false;
    }
    return true;
}

bool has_repeated_digits(ui n, ui n_digits, ui forbidden_digit)
{
    static bool appeared[10];
    for (ui i = 0; i < 10; i++)
        appeared[i] = false;
    appeared[forbidden_digit] = true;

    ui n_counted_digits = 0;

    while (n > 0)
    {
        ui digit = n % 10;
        if (appeared[digit])
            return true;
        appeared[digit] = true;
        n = n / 10;
        n_counted_digits += 1;
    }

    ui n_prefix_zeros = n_digits - n_counted_digits;
    if ((appeared[0] && n_prefix_zeros > 0) || (n_prefix_zeros > 1))
        return true;

    return false;
}

ui digits_to_number(const vector<ui>& digits)
{
    ui ans = 0;
    ui n_digits = digits.size();
    for (ui i = 0; i < n_digits; i++)
        ans = ans + pow10[i] * digits[i];
    return ans;
}

// placing the non-repeating digits
ui DFS(ui depth, vector<ui>& path, const ui& repeating_digit, const ui& n_repeating_digits, const ui& n_digits, const vector<ui>& primes)
{
    ui n_non_repeating_digits = path.size();

    if (depth == n_non_repeating_digits)
    {
        ui total = 0;
        if (repeating_digit == 0 && path[n_non_repeating_digits-1] != n_digits-1)
            return 0;
        vector<ui> digits = vector<ui>(n_digits, repeating_digit);
        for (ui i = 0; i < pow10[n_non_repeating_digits]; i++)
        {
            //if (!has_repeated_digits(i, n_non_repeating_digits, repeating_digit))
            {
                ui path_idx = 0;
                ui w = i;
                while (w > 0)
                {
                    digits[path[path_idx]] = w % 10;
                    path_idx += 1;
                    w = w / 10;
                }
                while (path_idx < n_non_repeating_digits)
                {
                    digits[path[path_idx]] = 0;
                    path_idx += 1;
                }
                if (digits[n_digits-1] == 0)
                    continue;
                ui n = digits_to_number(digits);
                if (is_prime(n, primes))
                    total = total + n;
            }
        }
        return total;
    }

    ui total = 0;

    ui place_lowerbound = 0;
    if (depth > 0)
        place_lowerbound = path[depth-1] + 1;
    for (ui place = place_lowerbound; place < n_digits; place++)
    {
        path[depth] = place;
        total = total + DFS(depth+1, path, repeating_digit, n_repeating_digits, n_digits, primes);
    }

    return total;
}


int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes(SQRT_MAX);

    for (ui repeating_digit = 0; repeating_digit <= 9; repeating_digit++)
    {
        for (ui n_repeating_digits = N-1; n_repeating_digits != 0; n_repeating_digits--)
        {
            vector<ui> path = vector<ui>(N - n_repeating_digits);
            ui sum = DFS(0, path, repeating_digit, n_repeating_digits, N, primes);
            ans = ans + sum;
            if (sum > 0)
                break;
        }
    }

    cout << ans << endl;

    return 0;
}
