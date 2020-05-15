// Question: https://projecteuler.net/problem=686

#include<iostream>
#include<cmath>

using namespace std;

int main()
{
    double log10_2 = log10(2);
    uint64_t N = 678910;
    uint64_t n = 0;
    uint64_t count = 0;

    while (true)
    {
        n = n + 1;
        // 2^n -> 10^(k + q) where k is an integer and q is in [0, 1)
        double nlog10_2 = n * log10_2;
        uint64_t k = uint64_t(nlog10_2);
        double q = nlog10_2 - k;
        double l = pow(10, q);
        if (l >= 1.23 and l < 1.24)
        {
            count = count + 1;
            if (count == N)
                break;
            n = n + 2;
        }
    }

    cout << n << "\n";

    return 0;
} 
