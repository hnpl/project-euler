// Question: https://projecteuler.net/problem=845

#include<iostream>
#include<vector>
#include<cmath>
#include<set>
#include<string>
#include<sstream>
#include<cassert>

using namespace std;

typedef uint64_t ui;

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = pow(10, 16);
//const ui N = 100000000;
//const ui N = 61;
const ui MAX_N_DIGITS = 18;
const ui MAX_SUM = 9 * MAX_N_DIGITS;

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

ui digit_sum(string s)
{
    ui ans = 0;
    for (auto digit: s)
        ans += digit - '0';
    return ans;
}

string solve(const ui& n)
{
    string ans = "";

    vector<ui> primes = get_primes(MAX_SUM);
    vector<vector<ui>> DP = vector<vector<ui>>(MAX_N_DIGITS+1, vector<ui>(MAX_SUM+1, 0));
    for (ui sum = 0; sum <= 9; sum++)
        DP[1][sum] = 1;
    for (ui n_digits = 2; n_digits <= MAX_N_DIGITS; n_digits++)
    {
        for (ui sum = 0; sum <= MAX_SUM; sum++)
        {
            for (ui next_digit = 0; next_digit <= 9; next_digit++)
            {
                DP[n_digits][sum+next_digit] += DP[n_digits-1][sum];
                assert((DP[n_digits][sum+next_digit] + DP[n_digits-1][sum]) < pow(10, 19));
            }
        }
    }

    set<ui> prime_set = set<ui>(primes.begin(), primes.end());
    //for (auto prime: prime_set)
    //{
    //    assert((ans + DP[MAX_N_DIGITS][prime]) < pow(10, 19));
    //    ans += DP[MAX_N_DIGITS][prime];
    //}
   
    // find out how many digits does the result have
    ui n_result_digits = 0;
    for (ui n_digits = 0; n_digits <= MAX_N_DIGITS; n_digits++)
    {
        ui count = 0;
        for (auto prime: prime_set)
            count += DP[n_digits][prime];
        if (count > N)
        {
            n_result_digits = n_digits;
            break;
        }
    }

    ans = n_result_digits;

    stringstream s;
    ui curr_sum = 0;
    ui n_counted_lowerbound = 0;
    for (ui n_built_digits = 0; n_built_digits < n_result_digits - 1; n_built_digits++)
    {
        ui n_remaining_digits = n_result_digits - n_built_digits - 1;
        ui next_count_lowerbound = n_counted_lowerbound;
        ui next_count_upperbound = n_counted_lowerbound;
        for (ui next_digit = 0; next_digit <= 9; next_digit++)
        {
            ui new_count = 0;
            next_count_lowerbound = next_count_upperbound;
            ui next_sum = curr_sum + next_digit;
            for (auto prime: primes)
            {
                if (prime < next_sum)
                    continue;
                //cout << next_sum << "+= DP["<<n_remaining_digits<<"][" << (prime-next_sum) <<"]"<< endl;
                new_count += DP[n_remaining_digits][prime - next_sum];
            }
            //cout << "next_digit: "<< next_digit << "; count: " << new_count << endl;
            next_count_upperbound = next_count_lowerbound + new_count;
            if (next_count_upperbound >= n)
            {
                s << next_digit;
                curr_sum += next_digit;
                n_counted_lowerbound = next_count_lowerbound;
                break;
            }
        }
        //cout << s.str() << " " << next_count_lowerbound << " " << next_count_upperbound << endl;
    }

    ans = s.str();

    for (ui next_digit = 0; next_digit <= 9; next_digit++)
    {
        stringstream candidate;
        candidate << ans << next_digit;
        ui candidate_digit_sum = digit_sum(candidate.str());
        if (prime_set.count(candidate_digit_sum) > 0)
            n_counted_lowerbound += 1;
        if (n_counted_lowerbound == n)
        {
            ans = candidate.str();
            break;
        }
    }

    return ans;
}

int main()
{
    auto ans = solve(N);
    cout << ans << endl;
    return 0;
}
