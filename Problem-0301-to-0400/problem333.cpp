// Question: https://projecteuler.net/problem=333

#include<iostream>
#include<array>
#include<vector>
#include<cmath>
#include<fstream>

typedef uint64_t ui;
#define endl "\n"

using namespace std;

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

//const ui N = pow(10, 2);
//const ui LARGEST_PRIME_N = 99;
const ui N = pow(10, 6);
const ui LARGEST_PRIME_N = 999983;
const ui MAX_EXP_2 = ui(floor(log(N)/log(2)));
const ui MAX_EXP_3 = ui(floor(log(N)/log(3)));

vector<ui> get_primes()
{
    vector<ui> primes;
    ifstream f("../inputs/primes_1e6.txt");
    ui p = 0;
    while (f >> p)
    {
        if (p > N)
            break;
        primes.push_back(p);
    }
    f.close();
    return primes;
}

ui count()
{
    ui ans = 0;

    vector<vector<vector<ui>>> dp = vector<vector<vector<ui>>>(N+1, vector<vector<ui>>(MAX_EXP_2+1, vector<ui>(MAX_EXP_3+1)));

    array<ui, MAX_EXP_2+1> pow2;
    array<ui, MAX_EXP_3+1> pow3;
    for (ui exp_2 = 0; exp_2 <= MAX_EXP_2; exp_2++)
        pow2[exp_2] = 1ULL << exp_2;
    pow3[0] = 1;
    for (ui exp_3 = 1; exp_3 <= MAX_EXP_3; exp_3++)
        pow3[exp_3] = 3 * pow3[exp_3-1];

    for (ui exp_2 = 0; exp_2 <= MAX_EXP_2; exp_2++)
    {
        ui curr_pow3 = 1;
        ui curr_pow2 = pow2[exp_2];
        ui exp_3 = 0;

        while (true)
        {
            ui p = curr_pow2 * curr_pow3;
            if (p > N)
                break;
            dp[p][exp_2][exp_3] = 1;
            curr_pow3 *= 3;
            exp_3 += 1;
        }
    }

    array<ui, LARGEST_PRIME_N+1> sum_count;
    fill(sum_count.begin(), sum_count.end(), 0);

    // Building numbers from terms with higher exponent of 2 and lower exponent of 3, to terms with lower exponent of 2 and higher exponent of 3
    for (ui curr_sum = 1; curr_sum <= LARGEST_PRIME_N; curr_sum++)
    {
        if (curr_sum % 1000 == 0)
            cout << curr_sum << endl;
        for (ui exp_2 = 0; exp_2 <= MAX_EXP_2; exp_2++)
        {
            ui curr_pow2 = pow2[exp_2];
            for (ui exp_3 = 0; exp_3 <= MAX_EXP_3; exp_3++)
            {
                ui curr_pow3 = pow3[exp_3];
                ui p = curr_pow2 * curr_pow3;
                if (p > N)
                    break;
                sum_count[curr_sum] += dp[curr_sum][exp_2][exp_3];
                // building the next numbers: adding terms with a lower exponent of 2 and a higher exponent of 3 to the current sum to build the next numbers
                for (ui next_exp_2 = 0; next_exp_2 < exp_2; next_exp_2++)
                {
                    for (ui next_exp_3 = exp_3+1; next_exp_3 <= MAX_EXP_3; next_exp_3++)
                    {
                        ui next_sum = curr_sum + pow2[next_exp_2] * pow3[next_exp_3];
                        if (next_sum > N)
                            break;
                        dp[next_sum][next_exp_2][next_exp_3] += dp[curr_sum][exp_2][exp_3];
                    }
                }
            }
        }
    }

    auto primes = get_primes();
    for (auto prime: primes)
        if (sum_count[prime] == 1)
            ans += prime;
    return ans;
}

int main()
{
    cout << count() << endl;
    return 0;
}

