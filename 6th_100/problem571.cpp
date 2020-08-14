// Problem: https://projecteuler.net/problem=571

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

bool k_is_pandigital_in_base_b(ui k, ui b)
{
    vector<bool> appeared = vector<bool>(b, false);
    while (k > 0)
    {
        appeared[k%b] = true;
        k = k / b;
    }
    return all_of(appeared.begin(), appeared.end(), [](bool i){return i;});
}

bool k_is_n_super_pandigital(ui k, ui n)
{
    for (ui base = n; base > 1; base--)
        if (!k_is_pandigital_in_base_b(k, base))
            return false;
    return true;
}

ui permutation_to_number(const vector<ui>& permutation, ui base)
{
    ui number = 0;
    ui coeff = 1;
    for (auto it = permutation.rbegin(); it != permutation.rend(); it++)
    {
        number = number + coeff * (*it);
        coeff = coeff * base;
    }
    return number;
}

int main()
{
    const ui base = 12;

    vector<ui> digits = vector<ui>(base);
    iota(digits.begin(), digits.end(), 0);

    ui count = 0;
    ui ans = 0;

    while (next_permutation(digits.begin(), digits.end()))
    {
        ui permutation = permutation_to_number(digits, base);
        if (k_is_n_super_pandigital(permutation, base))
        {
            ans = ans + permutation;
            count = count + 1;
            cout << count << " " << permutation << endl;
            if (count == 10)
                break;
        }
    }

    cout << ans << endl;

    return 0;
}
