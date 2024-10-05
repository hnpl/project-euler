// Question: https://projecteuler.net/problem=381
// g++ problem381.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -lpari -O3

/*
    Only consider odd primes.
    Wilson's Theorem:            (p-1)! = p-1 (mod p) for all prime p
    Obviously, it follows that   (p-2)! = 1 (mod p)
    So, ((p-1)! + (p-2)!) (mod p) = 0 -> we don't need to check (p-1)! mod p and (p-2)! mod p.

    Also, we have:
        (p-3)! * (p-2) * (p-1) [mod p] = (p-1) [mod p] = p-1
    ->  [(p-3)! [mod p] * (-2) * (-1)] [mod p] = p-1
    ->  [(p-3)! [mod p] * 2] [mod p] = p-1
    ->  (p-3)! [mod p] = floor(p/2) (we have that p is odd and p > 2, so (p-1)/2 is floor(p/2))

    Similarly,
        [(p-4)! * (p-3) * (p-2)] [mod p] = 1
    ->  [(p-4)! (mod p) * (-3) * (-2)] [mod p]= 1
    ->  [(p-4)! (mod p) * 6] [mod p] = 1
    ->  (p-4)! (mod p) = inverse_mod(6, p)

    Also,
        [(p-5)! * (p-4) * (p-3)] [mod p] = floor(p/2)
    ->  [(p-5)! (mod p) * (-4) * (-3)] [mod p]= floor(p/2)
    ->  [(p-5)! (mod p) * 12] [mod p] = floor(p/2)
    ->  (p-5)! (mod p) = [floor(p/2) * inverse_mod(12, p)] [mod p]

*/

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 100000000;


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

int main()
{
    ui ans = 0;
    auto primes = get_primes(N);

    auto prime_it = primes.begin() + 5; // ignore 2 and 3 and 5 and 7 and 11

    ans = 9; // S(5) + S(7) + S(11) = 9

    for (; prime_it != primes.end(); prime_it++)
    {
        ui p = *prime_it;
        ui t1;
        ui t2;
        //ui p_1 = p-1;
        //ui p_2 = 1;    so (p-1)%p + (p-2)!%p = p = 0 (mod p)
        ui p_3 = p / 2;
        ui p_4 = Fl_invgen(6, p, &t1);
        ui p_5 = p / 2 * Fl_invgen(12, p, &t2);
        ans = ans + (p_3 + p_4 + p_5) % p;
    }

    cout << ans << endl;

    return 0;
}