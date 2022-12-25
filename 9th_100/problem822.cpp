// Question: https://projecteuler.net/problem=822

// g++ -I$HOME/pari/include -l$HOME/pari/lib problem822.cpp -lpari -O3

/*
 * . Suppose that we have a list of base numbers, arr = [0, 1, 2, 3, ..., N] and we only operate on elements from index 2 to N.
 *   For each round, we will square the smallest number.
 * . It is not feasible to simulate all rounds given the #rounds is 10**16.
 * . However, given the constraints that we were squaring the number of base `k` to the exponent `exp` in the previous round, we can count how many rounds were played. This can be done by,
 *   . Suppose that the number base `k` were squared `m` times, then that number cam be expressed as,
 *      arr[k] = k**(2**m))
 *   . Suppose that in the previous round, arr[k] was the number that was updated.
 *   . Note that the most recently squared number is the largest number in the array `arr`, so, for all `j` in `arr`,
 *      arr[j] <= arr[k] (*)
 *   . However, the constraint (*) is not sufficient to determine the number of rounds that were played.
 *   . Note that, for after a round, we can have identical numbers in the array. There must be constraints to specify the order of which those identical number were updated.
 *     E.g., we can start with,
 *      arr = [..., 2, 3, 4, ..., 16] (round t_1)
 *      ... (some rounds) ...
 *      arr = [..., 4, 3, 4, ..., 16] (round t_2)
 *      ... (some rounds) ...
 *      arr = [..., 16, 9, 4, ..., 16] (round t_3)
 *      ... (some rounds) ...
 *      arr = [..., 16, 9, 16, ..., 16] (round t_4)
 *      Note that,
 *        * After round t_2, we can update either arr[2] or arr[4]. (**)
 *   . For simplicity, when we have a case as in (*), we will update the number of smaller base first. (***)
 *     . This results in we have one order of updating the array.
 *   . Given the value of `arr[k]`, for each `j` in [2, ..., N] such that `j != k`, we can find the number of times that `j` were squared by finding `t` such that,
 *     . (j**(2**t) < arr[k]) and (j**(2**(t+1)) > arr[k])
 *     or (j**(2**t) == arr[k] and j < k)
 *     or (j**(2**t) < arr[k] and (j**(2**(t+1)) == arr[k]) and j > k.
 *     The bottom two clauses are for satisfying (***).
 *   . Find such `t` can be done in linear time.
 *       We have that, log_2(log(arr[k])) = m + log(log(k)) / log(2)
 *       and, log_2(log(j**(2**t))) = t + log(log(j)) / log(2).
 *       So, we can find `t` by,
 *       ```
 *         let t = round_down(m + log(log(k)) / log(2) - log(log(j)) / log(2))
 *         if (m + log(log(k)) / log(2) == t + log(log(j)) / log(2)) then
 *           if k < j then t = t - 1
 *       ```
 *   . Then, the number of rounds played is the sum of the number of times each element in the array were updated.
 *   . So, we can iterate throught each element of the array and assume it to be the most recent updated element, then we can perform binary search to find whether that assumption yields the exact number of rounds we are looking for.
 */

#include <pari/pari.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

typedef int64_t ui;

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp - 1);
};

//const ui N = 5;
//const ui M = 3;
//const ui N = pow(10, 1);
//const ui M = pow(10, 2);
const ui N = pow(10, 4);
const ui M = pow(10, 16);
const ui MOD = 1234567891L;

inline long multiplicative_order(ui base, ui mod)
{
    return gtolong(znorder(gmodulss(base, mod), NULL)); // znorder(): find multiplicative order, gmodulss(x,y): find x mod y
}

ui pow_mod(ui base, ui exp, const ui& mod)
{
    ui ans = 1;

    while (exp > 0)
    {
        if (exp %2 == 1)
        {
            ans *= base;
            ans %= mod;
        }
        exp /= 2;
        base *= base;
        base %= mod;
    } 

    return ans;
}

ui calc()
{
    vector<double> log_2_of_log_ = vector<double>(N+1, 0);
    for (ui k = 2; k <= N; k++)
        log_2_of_log_[k] = log(log(k)) / log(2);

    ui ans = 0;
    ui ans_highest_base = 0;
    ui ans_highest_exp = 0;
    bool ans_valid = false;

    for (ui highest = 2; highest <= N; highest++)
    {
        ui highest_l = 1;
        ui highest_r = M;
        while (highest_l < highest_r)
        {
            ui highest_exp = (highest_l + highest_r) / 2;
            double highest_val = 1.0 * highest_exp + log_2_of_log_[highest];
            ui exp_sum = highest_exp;
            for (ui other = 2; other <= N; other++)
            {
                if (other == highest)
                    continue;
                double other_exp_d = highest_val - log_2_of_log_[other];
                ui other_exp = ui(other_exp_d);
                if ((highest_exp > 0) && (other_exp > 0) && abs(other_exp - other_exp_d) < 1e-6)
                {
                    if (other > highest)
                        other_exp -= 1;
                }
                exp_sum += other_exp;
                if (exp_sum > M)
                    break;
            }
            if (exp_sum > M)
                highest_r = highest_exp;
            else if (exp_sum < M)
                highest_l = highest_exp + 1;
            else
            {
                ans_highest_base = highest;
                ans_highest_exp = highest_exp;
                ans_valid = true;
                break;
            }
        }
        if (ans_valid)
            break;
    }

    vector<double> exp_of_pow_2_of_ = vector<double>(N+1, 0);
    double highest_val = ans_highest_exp + log_2_of_log_[ans_highest_base];
    exp_of_pow_2_of_[ans_highest_base] = ans_highest_exp;
    for (ui other = 2; other <= N; other++)
    {
        if (other == ans_highest_base)
            continue;
        double other_exp_d = highest_val - log_2_of_log_[other];
        ui other_exp = ui(other_exp_d);
        if ((other_exp > 0) && abs(other_exp - other_exp_d) < 1e-6)
        {
            if (other > ans_highest_base)
                other_exp -= 1;
        }
        exp_of_pow_2_of_[other] = other_exp;
    }

    vector<ui> multiplicative_order_of_base_mod_MOD = vector<ui>(N+1, 0);
    for (ui base = 2; base <= N; base++)
        multiplicative_order_of_base_mod_MOD[base] = multiplicative_order(base, MOD);

    vector<ui> exp_of_ = vector<ui>(N+1, 0);
    for (ui base = 2; base <= N; base++)
        exp_of_[base] = pow_mod(2, exp_of_pow_2_of_[base], multiplicative_order_of_base_mod_MOD[base]);

    vector<ui> evaluated_terms = vector<ui>(N+1, 0);
    for (ui base = 2; base <= N; base++)
        evaluated_terms[base] = pow_mod(base, exp_of_[base], MOD);

    for (ui base = 2; base <= N; base++)
    {
        ans += evaluated_terms[base];
        ans %= MOD;
    }

    ans %= MOD;

    return ans;
}

int main()
{
    pari_init(50000000,2);
    ui ans = calc();
    cout << ans << endl;
    pari_close();
    return 0;
}
