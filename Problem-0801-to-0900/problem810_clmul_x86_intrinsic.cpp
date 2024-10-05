// Question: https://projecteuler.net/problem=810

// Use Eratosthenes sieve for finding XOR-prime.
// Note that, given the numbers of bits of a and b, we know the numbers of bits of a (x) b.

// This is x86_64 specific
// g++ problem810_clmul_x86_intrinsic.cpp -O3 -mpclmul -march=native

#include<iostream>
#include<vector>
#include<immintrin.h>

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

ui GF2_prod(const ui& a, ui b)
{
    /*
    ui ans = 0;

    ui shifted_a = a;
    while (b > 0)
    {
        if (b % 2 == 1)
            ans ^= shifted_a;
        b /= 2;
        shifted_a <<= 1;
    }

    return ans;
    */
    __m128i A = _mm_cvtsi32_si128(a);
    __m128i B = _mm_cvtsi32_si128(b);
    __m128i C = __builtin_ia32_pclmulqdq128(A, B, 0);
    uint32_t ans = _mm_extract_epi32(C, 0);
    return (ui) ans;
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
