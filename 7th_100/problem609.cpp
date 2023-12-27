// Question: https://projecteuler.net/problem=609

/*
 * Ideas,
 *   - First, we compute prime_pi[p] for all p < N.
 *   - Then, we measure the length of each pi sequence. From the implementation, the maximum length is 12.
 *   - Then, we can use dynamic programming to determine how many sequences (including sequences with 1 element) starting from a number n having k non-prime elements. 
 *     Let q(n, k) be the number of sequences starting from the number n and having k non-prime elements.
 *     Then,
 *        . If n is a prime number,
 *            . If k == 0: q(n, k) = q(prime_pi[n], k) + 1 // we need to add the sequence (n) itself
 *            . If k != 0: q(n, k) = q(prime_pi[n], k)
 *        . If n is not a prime number
 *            . If k == 1: q(n, k) = q(prime_p[n], k-1) + 1 // we need to add the sequence (n) itself
 *            . If k != 1: q(n, k) = q(prime_p[n], k-1)
 *     The above relations are illustrated from the following example,
 *         . Suppose that we know that, for n=3,
 *           For k = 0, there are 2 sequences: (3) and (3,2)
 *           For k = 1, there is 1 seuqence: (3,2,1)
 *         . Consider n=5, a prime number.
 *           We know that prime_pi[5] = 3, and we can add the number 5 on top of each sequence of n=3. Now we have,
 *           For k = 0, there are 3 sequences: (5), (5,3), (5,3,2) // Note that we also add the (5) sequence itself
 *           For k = 1, there is 1 sequence: (5,3,2,1)
 *         . Now consider n=6, a non prime number.
 *           We know that prime_pi[6] = prime_pi[5] = 3, and we also add the number 6 on top of each sequence of n=3.
 *           This time, we have a different number of non-prime numbers in each sequence.
 *           For k = 0, there is no sequence.
 *           For k = 1, there are 3 sequences: (6), (6,3), (6,3,2)
 *           For k = 2, there is 1 sequence: (6,3,2,1)
 *    - We can construct q(n,k) for each n starting from 3 to n.
 *    - Note that, for a prime p >= 3, we have that q(p+1, k) = q(p+i, k) for all i such that (p+i) < next_prime[p].
 *      We do not implement this optimization because the without this optimization, the program is fast enough, and this optimization makes the code a lot less clear.
 */

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cassert>
#include<sstream>
#include<unordered_map>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return pow(base, exp-1) * base;
};

const ui N = pow(10, 8);
const ui MOD = pow(10, 9) + 7;

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

vector<ui> get_prime_pi(const vector<ui>& primes)
{
    vector<ui> prime_pi = vector<ui>(primes.back()+1, 0);
    const ui prime_pi_size = prime_pi.size();
    for (auto prime: primes)
        prime_pi[prime] = 1;
    for (ui i = 1; i < prime_pi_size; i++)
        prime_pi[i] += prime_pi[i-1];
    return prime_pi;
}

ui solve(const ui& n, const ui& mod)
{
    vector<ui> primes = get_primes(n);
    vector<bool> is_prime = vector<bool>(n+1, false);
    for (auto prime: primes)
        is_prime[prime] = true;

    vector<ui> prime_pi = get_prime_pi(primes);
    ui max_prime_pi = 0;
    while (prime_pi.size() < n+1)
        prime_pi.push_back(prime_pi.back());
    for (auto p: prime_pi)
        max_prime_pi = max(max_prime_pi, p);

    vector<ui> u_seq_length = vector<ui>(n+1, 0);
    vector<ui> u_seq_length_primes= vector<ui>(n+1, 0);
    u_seq_length[0] = 0;
    u_seq_length[1] = 1;
    ui max_seq_length = 0;
    for (ui u = 2; u <= n; u++) // prime_pi[u] < u so u_seq_length[prime_pi[u]] is always determined before u_seq_length[u]
    {
        u_seq_length[u] = u_seq_length[prime_pi[u]]+1;
        u_seq_length_primes[u] = u_seq_length_primes[prime_pi[u]];
        if (is_prime[u])
            u_seq_length_primes[u] += 1;
        max_seq_length = max(max_seq_length, u_seq_length[u]);
    }

    vector<vector<ui>> p = vector<vector<ui>>(max_prime_pi+1, vector<ui>(max_seq_length+1, 0));
    vector<ui> col_sum = vector<ui>(max_seq_length+1, 0);
    p[1][1] = 1;
    p[2][0] = 1;
    p[2][1] = 1;
    col_sum[0] = p[1][0] + p[2][0];
    col_sum[1] = p[1][1] + p[2][1];
    for (ui u = 3; u <= max_prime_pi; u++)
    {
        if (!is_prime[u])
        {
            for (ui prev_k = 0; prev_k < max_seq_length; prev_k++)
                p[u][prev_k+1] = p[prime_pi[u]][prev_k];
            p[u][1] += 1; // adding the (u) sequence itself
        }
        else
        {
            for (ui prev_k = 0; prev_k <= max_seq_length; prev_k++)
                p[u][prev_k] = p[prime_pi[u]][prev_k];
            p[u][0] += 1; // adding the (u) sequence itself
        }
        for (ui k = 0; k <= max_seq_length; k++)
        {
            col_sum[k] += p[u][k];
            col_sum[k] %= mod;
        }
    }
    vector<ui> curr_row = vector<ui>(max_seq_length+1, 0);
    for (ui u = max_prime_pi+1; u <= n; u++)
    {
        fill(curr_row.begin(), curr_row.end(), 0);
        if (!is_prime[u])
        {
            for (ui prev_k = 0; prev_k < max_seq_length; prev_k++)
                curr_row[prev_k+1] = p[prime_pi[u]][prev_k];
            curr_row[1] += 1; // adding the (u) sequence itself
        }
        else
        {
            for (ui prev_k = 0; prev_k <= max_seq_length; prev_k++)
                curr_row[prev_k] = p[prime_pi[u]][prev_k];
            curr_row[0] += 1; // adding the (u) sequence itself
        }
        for (ui k = 0; k <= max_seq_length; k++)
        {
            col_sum[k] += curr_row[k];
            col_sum[k] %= mod;
        }
    }
    //col_sum[0] -= prime_pi[n]; // col_sum[0] and [1] are unsigned integers; both need adjustments for modular arithmetic
    //col_sum[1] -= n - prime_pi[n]
    col_sum[0] = ((col_sum[0] + mod) - prime_pi[n]) % mod;
    col_sum[1] = ((col_sum[1] + mod) - n + prime_pi[n]) % mod; // mod > n so this is safe
    for (ui i = 0; i <= max_seq_length; i++)
        cout << col_sum[i] << " ";
    cout << endl;
    ui ans = 1;
    for (ui i = 0; i < max_seq_length; i++)
    {
        if (col_sum[i] > 0)
        {
            ans *= col_sum[i];
            ans %= mod;
        }
    }
    return ans;
}

int main()
{
    ui ans = solve(N, MOD);
    cout << ans << endl;
    return 0;
}

