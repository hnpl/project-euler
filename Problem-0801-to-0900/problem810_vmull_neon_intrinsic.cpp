// Question: https://projecteuler.net/problem=810

// Use Eratosthenes sieve for finding XOR-prime.
// Note that, given the numbers of bits of a and b, we know the numbers of bits of a (x) b.

// This is specific to ARM machine with NEON
// g++ problem810_vmull_neon_intrinsic.cpp -O3 -march=native

#include<iostream>
#include<vector>
#include<arm_neon.h>
#include<arm_acle.h>
#include<inttypes.h>
using namespace std;

typedef uint32_t ui;
#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = 5000000;
const ui MAX_N_BITS = 28;
const ui SIEVE_UPPERBOUND = pow(2, MAX_N_BITS) - 1;

//https://graphics.stanford.edu/~seander/bithacks.html
static const int MultiplyDeBruijnBitPosition[32] = 
{
    0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
    8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
};
inline ui fast_log2(ui v)
{
    ui r = 0;

    v |= v >> 1; // first round down to one less than a power of 2 
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    r = MultiplyDeBruijnBitPosition[(ui)(v * 0x07C4ACDDU) >> 27];
    return r;
};

ui GF2_prod(const ui& a, const ui& b)
{
    poly64_t x = a;
    poly64_t y = b;
    return (ui)vmull_p64(x, y);
}

vector<bool> do_sieving(const ui& upperbound)
{
    ui n_elements = (upperbound - 3) / 2 + 1; // we only consider odd numbers starting from 3
    vector<bool> sieve = vector<bool>(n_elements, true);

    ui element_1 = 3;
    ui element_1_idx = 0;
    for (; element_1 <= upperbound; element_1+=2, element_1_idx++)
    {
        if (!sieve[element_1_idx])
            continue;
        ui n_bits_element_1 = fast_log2(element_1);
        ui max_n_bits_element_2 = MAX_N_BITS - n_bits_element_1;
        if (max_n_bits_element_2 == 0)
            continue;
        ui max_element_2 = (1UL << max_n_bits_element_2) - 1;
        for (ui element_2 = 3; element_2 <= max_element_2; element_2 += 2)
        {
            //cout << element_1 << " " << element_2 << endl;
            ui product = GF2_prod(element_1, element_2);
            ui product_idx = (product - 3) / 2;
            if (product <= upperbound)
                sieve[product_idx] = false;
        }
    }

    return sieve;
}

int main()
{
    auto sieve = do_sieving(SIEVE_UPPERBOUND);
    ui count = 0;
    ui n = sieve.size();
    ui idx = 0;
    for (; idx < n; idx++)
    {
        if (sieve[idx])
        {
            count++;
            if (count == N-1)
                break;
        }
    }
    ui ans = idx * 2 + 3;
    cout << ans << endl;
    return 0;
}
