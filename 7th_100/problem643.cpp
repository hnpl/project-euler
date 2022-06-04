// Question: https://projecteuler.net/problem=643

/*
 * Stategy:
 *   - If a and b are 2-friendly (meaning, gcd(a,b)=2^t), then gcd(a/2^t, b/2^t)=1. I.e., a/2^t and b/2^t are coprimes.
 *   - So, for every value 2^t, we can count how many pairs of p and q such that p < q < N/2^t and gcd(p,q)=1.
 *   - According to https://math.stackexchange.com/questions/423848/number-of-ordered-pairs-of-coprime-integers-from-1-to-n,
 *      . The number of coprime pairs less than K is sum_{j = 1..K} Euler_phi(j)
 *   - So, the answer to the question is,
 *      . sum_{t=1..floor(log(N)/log(2))} [sum_{j=1..N/(2^t)} [Euler_phi(j)] - 1]
 *   - There are algorithms that calculate sum of Euler_phi in less than O(N) time, but I implemented an essentially O(N^2) algorithm, so it's very slow.
 */
#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<cassert>
#include<thread>
#include<sstream>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(ui base, ui exp)
{
    if (exp == 0)
        return 1;
    return pow(base, exp-1) * base;
};

//const ui N = pow(10, 11);
const ui N = 1000000;
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

void get_phi_sum(const ui start, const ui length,
                 const ui thread_idx,
                 const vector<ui>& primes, const vector<ui>& prime_inv_mod,
                 const vector<ui>& considering_ranges,
                 vector<ui>& whole_segment_sum, vector<ui>& result)
{
    ui end = start + length - 1;
    vector<ui> phi(length, 1);

    for (ui i = 0; i < primes.size(); i++)
    {
        ui prime = primes[i];
        if (prime > start + length - 1)
            ;
        ui prime_inv = prime_inv_mod[i];
        __uint128_t t = (prime - 1) * prime_inv;
        t %= MOD;
        ui offset = prime - start % prime;
        if (offset == prime)
            offset = 0;
        if (thread_idx == 71)
        {
            if (prime < 20)
                cout << prime << " " << offset << endl;
        }
        if (start == 0 && offset == 0)
            offset = 2*prime;
        for (ui j = offset; j < length; j+=prime)
        {
            __uint128_t t2 = t * phi[j];
            t2 %= MOD;
            phi[j] = t2;
        }
    }

    for (ui i = 0; i < length; i++)
    {
        if (phi[i] == 1) // start + i is prime
        {
            phi[i] = start + i - 1;
        }
        else
        {
            __uint128_t t = (__uint128_t)phi[i] * (start + i);
            phi[i] = t % MOD;
        }
    }

    if (start == 0)
    {
        phi[0] = 0;
        if (length > 1)
            phi[1] = 1;
    }
    if (start == 1)
    {
        phi[0] = 1;
    }

    if (thread_idx == 71)
        for (ui i = 0; i <= min(20, (int)length-1); i++)
            cout << "phi[" << start+i << "] = " << phi[i] << endl;

    for (ui i = 1; i < length; i++)
        phi[i] = (phi[i] + phi[i-1]) % MOD;

    for (ui r_idx = 0; r_idx < considering_ranges.size(); r_idx++)
    {
        ui r = considering_ranges[r_idx];
        if (r >= start && r <= end)
        {
            assert(result[r_idx]==0);
            result[r_idx] = phi[r - start];
            stringstream s;
            s << "Thread " << thread_idx << ": " << r << "->" << result[r_idx] << endl;
            cout << s.str();
        }
    }
    
    whole_segment_sum[thread_idx] = phi[length-1];
    stringstream s;
    s << "Calculating phi from " << start << " to " << start + length - 1 << ": " << whole_segment_sum[thread_idx]<< endl;
    cout << s.str();
}

ui inv_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

int main()
{
    ui prime_upperbound = N+1;
    auto primes = get_primes(prime_upperbound);
    vector<ui> prime_inv_mod;
    for (auto p: primes)
        prime_inv_mod.push_back(inv_mod(p, MOD));

    vector<ui> considering_ranges;

    ui base = 2;
    while (base <= N)
    {
        ui p = N / base;
        considering_ranges.push_back(p);
        base *= 2;
    }
    reverse(considering_ranges.begin(), considering_ranges.end());

    vector<ui> result(considering_ranges.size(), 0);

    ui n_threads = 80;
    ui length_per_thread = (N/2 + 1 + n_threads) / n_threads;
    vector<thread> threads;
    vector<ui> whole_segment_sum(n_threads, 0);

    vector<ui> thread_start;
    vector<ui> thread_end;

    ui thread_idx = 0;

    for (ui segment_start = 0; segment_start <= N/2; segment_start += length_per_thread)
    {
        thread_start.push_back(segment_start);
        thread_end.push_back(segment_start+min(length_per_thread, N/2 - segment_start + 1)-1);
        assert(thread_end.back() >= 0);
        thread new_thread(get_phi_sum,
                          segment_start, min(length_per_thread, (N/2 - segment_start + 1)),
                          thread_idx,
                          ref(primes), ref(prime_inv_mod),
                          ref(considering_ranges),
                          ref(whole_segment_sum), ref(result));
        threads.push_back(move(new_thread));
        thread_idx += 1;
    }

    for (ui i = 0; i < threads.size(); i++)
        threads[i].join();


    ui idx = 0;
    ui curr_start = thread_start[idx];
    ui curr_end = thread_end[idx];
    ui curr_sum = 0;
    vector<ui> result_sum;
    ui prev_whole_segment_sum = 0;
    for (ui r_idx = 0; r_idx < considering_ranges.size(); r_idx++)
    {
        ui r = considering_ranges[r_idx];
        while (!(r >= curr_start && r <= curr_end))
        {
            curr_start = thread_start[idx];
            curr_end = thread_end[idx];
            curr_sum += whole_segment_sum[idx];
            prev_whole_segment_sum = whole_segment_sum[idx];
            idx++;
        }
        cout <<curr_start << " " << curr_end << " " << curr_sum << " " << endl;
        result_sum.push_back((curr_sum - prev_whole_segment_sum + result[r_idx]) % MOD);
    }
    assert(idx >= threads.size());
    for (ui i = 0; i < considering_ranges.size(); i++)
        cout << "(" << i << ")" << " " << result[i] << " ";
    cout << endl;
    int64_t ans = 0;
    for (int64_t r_idx = considering_ranges.size() - 1; r_idx >= 0; r_idx--)
    {
        ans += result_sum[r_idx] - 1;
        ans %= MOD;
    }
    for (ui r_idx = 0; r_idx < considering_ranges.size(); r_idx++)
        cout << considering_ranges[r_idx] << " " << result_sum[r_idx] << endl;
    cout << ans << endl;
    return 0;

}

