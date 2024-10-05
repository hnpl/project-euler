// Question: https://projecteuler.net/problem=233

// - Ref: https://math.stackexchange.com/questions/108267/for-what-kind-of-numbers-would-r-2n2-420
//        https://mathworld.wolfram.com/SumofSquaresFunction.html (equation 15, 16)
// - Though, contradicting to the references, it seems that the equations should be,
//     . If n = 2^a_0 * p_1 ^ a_1 * ... * p_m ^ a_m * q_1 ^ b_1 * ... * q_n ^ b_n [1]
//       then the number of expessing n as a sum of 2 squares is
//            B = 4 * (2(b_1) + 1) * ... * (2(b_n) + 1)
//       where p_i are primes of form 4k+3, and q_i are primes of form 4k+1.
// - We have that B = 420, then (2(b_1) + 1) * ... * (2(b_n) + 1) = 105
//   So, there are a few possibilities for {b_i},
//     . {1, 2, 3}
//     . {1, 17}
//     . {2, 10}
//     . {3, 7}
//     . {52}
//  - For each combination of {b_i}, we can find all combinations of q_1 ^ b_1 * ... * q_n ^ b_n using brute force.
//  - Let x = n / (q_1 ^ b_1 * ... * q_n ^ b_n)
//    Due to equation [1], x is a number that does not have any prime factor of form 4k+1.
//    It is showed that x <= 278454 (this equals to 10**11 / min_prod, where min_prod is the smallest possible value of q_1 ^ b_1 * ... * q_n ^ b_n,
//    which is 5^3 * 13^2 * 17^1).
//    This number is small enough that we can create a sieve filtering out all numbers that is divisible by any prime of form 4k+1.
//    Then we can create a prefix sum array, where prefix_sum[i] = sum of all numbers that is smaller than i and not divisible by any prime of form 4k+1.

#include<algorithm>
#include<cmath>
#include<iostream>
#include<numeric>
#include<unordered_set>
#include<vector>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
}

ui pow_fast(ui base, ui exp)
{
    ui ans = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            ans *= base;
        base *= base;
        exp /= 2;
    }
    return ans;
}

const ui N = pow(10, 11);
const double LOG_N = log(N);
const ui M = 420;

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

//ui min_prod = -1ULL;

ui dfs_4k1(const ui& depth, const ui& max_depth,
           vector<ui>& prime_factor_path_4k1, const vector<ui>& exp_path_4k1,
           const double& curr_log, const double& max_log,
           const ui& curr_prod, const ui& max_prod,
           const vector<ui>& primes_4k1, const vector<double>& primes_4k1_log,
           const vector<ui>& primes_4k3_prefix_sum)
{
    if (depth == max_depth)
    {
        if (false)
        {
            for (ui i = 0; i<max_depth;  i++)
                cout << primes_4k1[prime_factor_path_4k1[i]] << "^"<< ((exp_path_4k1[i]-1)/2) << " * " ;
            cout << "=" << curr_prod << N /curr_prod<<endl;
        }
        //min_prod = min(min_prod, curr_prod);
        return curr_prod * primes_4k3_prefix_sum[N / curr_prod];
    }
    ui ans = 0;
    unordered_set<ui> used_4k1_prime_idx;
    for (ui i = 0; i < depth; i++)
        used_4k1_prime_idx.insert(prime_factor_path_4k1[i]);
    ui curr_exp = (exp_path_4k1[depth] - 1) / 2;
    for (ui i = 0; i < primes_4k1.size(); i++)
    {
        //cout << "Depth=" << depth << "; Considering: " << primes_4k1[i] << "^" << curr_exp << endl;
        ui prime = primes_4k1[i];
        double prime_log = primes_4k1_log[i];
        if (used_4k1_prime_idx.find(i) != used_4k1_prime_idx.end())
            continue;
        double next_log = curr_log + prime_log * curr_exp;
        //if (depth == max_depth-1)
            //cout << "Curr log: " << curr_log << "; Next log: " << next_log << "; Max log: " << max_log << endl;
        if (next_log - 1 > max_log)
            break;
        ui next_prod = curr_prod * pow_fast(prime, curr_exp);
        if (next_prod > max_prod)
            break;
        prime_factor_path_4k1[depth] = i;
        ans += dfs_4k1(depth+1, max_depth,
                       prime_factor_path_4k1, exp_path_4k1,
                       next_log, max_log,
                       next_prod, max_prod,
                       primes_4k1, primes_4k1_log,
                       primes_4k3_prefix_sum);
    }
    return ans;
}

// Given a set of exponents for 4k1 primes {a_1, a_2, ..., a_n},
// we find all possible number q such that q <= N and q = q_1^a_1 * q_2^a_2 * ... * q_n^a_n
// where q_i are 4k+1 primes
ui count_accounting_4k1(const vector<ui>& exp_path_4k1, const ui& path_depth,
                        const vector<ui>& primes_4k1, const vector<double>& primes_4k1_log,
                        const vector<ui>& primes_4k3_prefix_sum)
{
    vector<ui> path = vector<ui>(path_depth, 0);
    ui ans = dfs_4k1(0, path_depth,
                     path, exp_path_4k1,
                     0, LOG_N,
                     1, N,
                     primes_4k1, primes_4k1_log,
                     primes_4k3_prefix_sum);
    return ans;
}

// Find set of exponents for 4k1 primes
ui dfs(const ui& depth, vector<ui>& path, const ui& curr_prod, const ui& target_prod,
       const vector<ui>& primes_4k1, const vector<double>& primes_4k1_log,
       const vector<ui>& primes_4k3_prefix_sum)
{
    if (curr_prod == target_prod)
    {
        //{
        //    for (ui i = 0; i<depth; i++)
        //        cout << (path[i]-1)/2 << " , " ;
        //    cout << endl;
        //}
        return count_accounting_4k1(path, depth, primes_4k1, primes_4k1_log, primes_4k3_prefix_sum);
    }
    ui ans = 0;
    ui remaining_prod = target_prod / curr_prod;
    ui remaining_prod_divisor = 2;
    if (depth > 0)
        remaining_prod_divisor = path[depth-1];
    for (; remaining_prod_divisor <= remaining_prod; remaining_prod_divisor++)
    {
        if (remaining_prod % remaining_prod_divisor != 0)
            continue;
        path[depth] = remaining_prod_divisor;
        ans += dfs(depth+1, path, curr_prod * remaining_prod_divisor, target_prod, primes_4k1, primes_4k1_log, primes_4k3_prefix_sum);
    }
    return ans;
}

void pre_sum_primes_4k3(vector<ui>& primes_4k3_prefix_sum, const vector<ui>& primes_4k1, const vector<ui>& primes_4k3)
{
    const ui limit = primes_4k3_prefix_sum.size() - 1;
    iota(primes_4k3_prefix_sum.begin(), primes_4k3_prefix_sum.end(), 0); // filling primes_4k3_prefix_sum with 0, 1, 2, ..., limit
    primes_4k3_prefix_sum[0] = 0;
    for (auto prime: primes_4k1)
        for (ui w = prime; w <= limit; w+=prime)
            primes_4k3_prefix_sum[w] = 0;
    for (ui i = 1; i <= limit; i++)
        primes_4k3_prefix_sum[i] += primes_4k3_prefix_sum[i-1];
}

ui solve(const ui& n, ui m)
{
    ui ans = 0;
    m /= 4;
    ui primes_4k1_limit = N / (5*5*5*13*13); // The exponents for 4k1 primes are in { (1,2,3), (1,17), (2,10), (3,7), (52) } (this is result of dfs())
                                             // So, the largest primes to fit in one of the primes tuple is N / (5^3 * 13^2)
    vector<ui> primes = get_primes(primes_4k1_limit);
    vector<ui> primes_4k1;
    vector<ui> primes_4k3;
    vector<double> primes_4k1_log;
    vector<ui> primes_4k3_prefix_sum;
    primes_4k1.reserve(primes.size() / 2 + 100);
    primes_4k3.reserve(primes.size() / 2 + 100);
    primes_4k1_log.reserve(primes.size() / 2 + 100);
    for (auto prime: primes)
    {
        if (prime % 4 == 1)
        {
            primes_4k1.push_back(prime);
            primes_4k1_log.push_back(log(prime));
        }
        else
        {
            primes_4k3.push_back(prime);
        }
    }
    primes_4k3_prefix_sum = vector<ui>(278455, 0); // N/min_prod = 278454, so we only need to count
                                                   // numbers that do not contain 4k+1 prime factor
                                                   // upto 278454
    pre_sum_primes_4k3(primes_4k3_prefix_sum, primes_4k1, primes_4k3);
    vector<ui> path = vector<ui>(10, 0);
    ans = dfs(0, path, 1, m, primes_4k1, primes_4k1_log, primes_4k3_prefix_sum);
    //cout << min_prod << " " << N / min_prod << endl;
    return ans;
}

int main()
{
    ui ans = solve(N, M);
    cout << ans << endl;
    return 0;
}
