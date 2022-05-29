// Question: https://projecteuler.net/problem=722

/*
    - Strategy:
        . The sum can be rewritten as,
            S = sum_{n=1}^{inf} [sigma_k(n) * q^n] // for some |q| < 1
              = sigma_k(1) * q^1              + sigma_k(2) * q^2  + sigma_k(3) * q^3  + sigma_k(4) * q^4        + ...
              = 1^k * q^1                     + (1^k + 2^k) * q^2 + (1^k + 3^k) * q^3 + (1^k + 2^k + 4^k) * q^4 + ...
              = 1^k * (q^1 + q^2 + q^3 + ...) + 2^k * (q^2 + q^4 + q^6 + ...) + 3^k * (q^3 + q^6 + q^9 + ...)   + ...
              = 1^k * sum_j(q^j)              + 2^k * sum_j((q^2)^j)          + 3^k * sum_j((q^3)^j)            + ...
              = 1^k * (q^1)/(1-q^1)           + 2^k * (q^2) / (1-q^2)         + 3^k * (q^3) / (1-q^3)           + ... // geometric series sum
              = sum_{j=1}{inf} (j^k * (q^j)/(1-q^j))
            which can be computed in parallel.
        . As a side note, the sum is in the form of a Lambert series (https://en.wikipedia.org/wiki/Lambert_series).
 */

#include<iostream>
#include<cmath>
#include<thread>
#include<vector>
#include<array>
#include<cassert>
#include<iomanip>

using namespace std;

typedef long double df;
typedef uint64_t ui;

const ui n_threads = 80;
const ui E = 5000000000;

void work(ui k_start, ui k_end, ui index, df& results)
{
    const df alpha = 1ULL << 25;
    const df beta = 1.0 - 1.0/alpha;
    for (ui k = k_start; k <= k_end; k++)
        results += pow(k, 15) * (1.0/((1.0/pow(beta, k))-1.0));
}

int main()
{
    df ans = 0;
    
    vector<thread> threads;
    array<df, n_threads*64*64> results;
    fill(results.begin(), results.end(), 0);

    ui work_amount = (E ) / n_threads;
    ui k_start = 1;
    ui k_end = work_amount-1;
    for (ui i = 0; i < n_threads; i++)
    {
        thread new_thread(work, k_start, k_end, i, ref(results[i*64*64]));
        threads.push_back(move(new_thread));
        k_start = k_end + 1;
        k_end = min((i+1) * work_amount - 1, E);
        cout << i << " " << k_start << " " << k_end << endl;
    }

    for (ui i = 0; i < n_threads; i++)
        threads[i].join();

    ans = 0.0;
    for (ui k = 0; k < n_threads; k++)
    {
        cout << k << " " << results[k*64*64] << ans << endl;
        ans += results[k*64*64];
    }

    cout <<  setprecision(18) << ans << endl;

    return 0;
}
