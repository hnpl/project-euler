// Question: https://projecteuler.net/problem=251
// g++ test.cpp -O3 -march=native -pthread

#include<iostream>
#include<vector>
#include<cmath>
#include<queue>
#include<cassert>
#include<utility>
#include<unordered_map>
#include<string>

#include<thread> //sigh


using namespace std;

typedef __int128 Z128;

#define endl "\n"

const Z128 N = 110000000;

const Z128 MAX_P = N; 

const Z128 n_threads = 4;

// We know that a mod 3 = 2. Let a = 3p + 2, then we have b^2 * c = (p+1)^2 * (8p+5)


vector<Z128> get_primes()
{
    vector<Z128> primes;

    vector<bool> is_prime = vector<bool>(MAX_P + 1, true);
    for (Z128 p = 2; p <= MAX_P; p++)
    {
        if (!is_prime[p])
            continue;
        primes.push_back(p);
        for (Z128 k = 2*p; k <= MAX_P; k+=p)
            is_prime[k] = 0;
    }
    return primes;
}

vector<Z128> get_squares()
{
    vector<Z128> squares;
    for (Z128 i = 1; i <= N; i++)
        squares.push_back(i*i);
    return squares;
}

void prime_factorize(Z128 n, const vector<Z128>& primes, vector<Z128>& factor, vector<Z128>& power)
{
    for (auto p: primes)
    {
        if (p*p > n)
            break;
        if (n%p == 0)
        {
            factor.push_back(p);
            power.push_back(0);
            while (n%p == 0)
            {
                n = n / p;
                power.back()+=1;
            }
        }
    }

    if (n!=1 && factor.size() == 0)
    {
        factor.push_back(n);
        power.push_back(1);
    }
}

void merge_factors(vector<Z128>& factor, vector<Z128>& power, const vector<Z128>& factor_p1, const vector<Z128>& power_p1,
                                                              const vector<Z128>& factor_p2, const vector<Z128>& power_p2)
{
    Z128 i1, i2;
    i1 = 0;
    i2 = 0;
    auto n1 = factor_p1.size();
    auto n2 = factor_p2.size();

    while ((i1 < n1) && (i2 < n2))
    {
        if (factor_p1[i1] < factor_p2[i2])
        {
            if (power_p1[i1] > 1)
            {
                factor.push_back(factor_p1[i1]);
                power.push_back(power_p1[i1]);
            }
            i1 = i1 + 1;
        }
        else if (factor_p1[i1] > factor_p2[i2])
        {
            if (power_p2[i2] > 1)
            {
                factor.push_back(factor_p2[i2]);
                power.push_back(power_p2[i2]);
            }
            i2 = i2 + 1;
        }
        else
        {
            factor.push_back(factor_p1[i1]);
            power.push_back(power_p1[i1] + power_p2[i2]);
            i1 = i1 + 1;
            i2 = i2 + 1;
        }
    }
    while (i1 < n1)
    {
        if (power_p1[i1] > 1)
        {
            factor.push_back(factor_p1[i1]);
            power.push_back(power_p1[i1]);
        }
        i1 = i1 + 1;
    }
    while (i2 < n2)
    {
        if (power_p2[i2] > 1)
        {
            factor.push_back(factor_p2[i2]);
            power.push_back(power_p2[i2]);
        }
        i2 = i2 + 1;
    }

}

void find_square_factors(vector<Z128>& b2, vector<Z128>& b, const vector<Z128>& factor, const vector<Z128>& power)
{
    auto n = factor.size();

    queue<Z128> bfs;
    queue<Z128> next_index;
    queue<Z128> sqr;


    for (Z128 i = 0; i < n; i++)
    {
        Z128 square = factor[i] * factor[i];
        Z128 w = 1;
        Z128 t = 1;
        for (Z128 j = 2; j <= power[i]; j+=2)
        {
            w = w * square;
            t = t * factor[i];
            if (t > N)
                break;
            b2.push_back(w);
            b.push_back(t);
            for (Z128 next = i+1; next < n; next++)
            {
                bfs.push(w);
                sqr.push(t);
                next_index.push(next);
            }
        }
    }

    while (!bfs.empty())
    {
        auto f = bfs.front();
        bfs.pop();
        auto s = sqr.front();
        sqr.pop();
        auto i = next_index.front();
        next_index.pop();

        Z128 square = factor[i] * factor[i];
        Z128 w = f;
        Z128 t = s;

        for (Z128 j = 2; j <= power[i]; j+=2)
        {
            w = w * square;
            t = t * factor[i];
            if (t > N)
                break;
            b2.push_back(w);
            b.push_back(t);
            for (Z128 next = i+1; next < n; next++)
            {
                bfs.push(w);
                sqr.push(t);
                next_index.push(next);
            }
        }
    }
}

void count_triplets(Z128 start, Z128 end, const vector<Z128>& primes, Z128& ans)
{
    for (Z128 p = start; p <= end; p++)
    {
        Z128 a = 3*p+2;
        Z128 p1 = (p+1)*(p+1);
        Z128 p2 = 8*p+5;
        Z128 b2c = p1*p2;
        
        vector<Z128> factor_p1;
        vector<Z128> power_p1;
        prime_factorize(p1, primes, factor_p1, power_p1);
        vector<Z128> factor_p2;
        vector<Z128> power_p2;
        prime_factorize(p2, primes, factor_p2, power_p2);

        vector<Z128> factor;
        vector<Z128> power;
        merge_factors(factor, power, factor_p1, power_p1, factor_p2, power_p2);

        vector<Z128> b2;
        vector<Z128> b;
        find_square_factors(b2, b, factor, power);
        b2.push_back(1);
        b.push_back(1);
        Z128 n = b2.size();
        for (Z128 i = 0; i < n; i++)
        {
            Z128 c = b2c / b2[i];
            if ((a + b[i] + c <= N))
                ans = ans + 1;
        }
    }
    string s = to_string((int)start) + "," + to_string((int)end) + "," + to_string((int)ans) + "\n";
    cout << s;
}

int main()
{
    Z128 ans = 0;
    
    auto primes = get_primes();

    vector<thread> threads;
    threads.reserve(n_threads);
    Z128 values[n_threads];
    for (Z128 i = 0; i < n_threads; i++)
        values[i] = 0;

    Z128 p_per_job = (N/6)/n_threads;
    Z128 start = 0;
    Z128 end = start + p_per_job;

    for (Z128 i = 0; i < n_threads; i++)
    {
        thread new_thread(count_triplets, start, end, ref(primes), ref(values[i]));
        threads.push_back(move(new_thread));
        start = end + 1;
        end = min(N/6-1, start + p_per_job);
    }

    for (Z128 i = 0; i < n_threads; i++)
        threads[i].join();

    for (Z128 i = 0; i < n_threads; i++)
        ans = ans + values[i];


    /*
    for (Z128 p = 0; p < N/6; p++)
    {
        Z128 a = 3*p+2;
        Z128 p1 = (p+1)*(p+1);
        Z128 p2 = 8*p+5;
        Z128 b2c = p1*p2;
        
        vector<Z128> factor_p1;
        vector<Z128> power_p1;
        prime_factorize(p1, primes, factor_p1, power_p1);
        vector<Z128> factor_p2;
        vector<Z128> power_p2;
        prime_factorize(p2, primes, factor_p2, power_p2);

        vector<Z128> factor;
        vector<Z128> power;
        merge_factors(factor, power, factor_p1, power_p1, factor_p2, power_p2);
        
        
        // vector<Z128> factorq;
        // vector<Z128> powerq;
        // prime_factorize(b2c, primes, factorq, powerq);
        // vector<Z128> factor;
        // vector<Z128> power;
        // for (Z128 i = 0; i < factorq.size(); i++)
        //     if (powerq[i] > 1)
        //     {
        //        factor.push_back(factorq[i]);
        //         power.push_back(powerq[i]);
        //     }
        

        vector<Z128> b2;
        vector<Z128> b;
        find_square_factors(b2, b, factor, power);
        b2.push_back(1);
        b.push_back(1);
        Z128 n = b2.size();
        for (Z128 i = 0; i < n; i++)
        {
            Z128 c = b2c / b2[i];
            if ((a + b[i] + c <= N))
                ans = ans + 1;
        }
        //cout << p << " " << n << endl;
        if (p%10000 == 0)
            cout << (int64_t)p  << " "<< (int64_t)ans<<endl;
    }
    */

    cout << (int64_t)ans << endl;

    return 0;
}