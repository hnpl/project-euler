// Question: https://projecteuler.net/problem=588

// See: 6th_100/problem588.py

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <string>
#include <bitset>
#include <cassert>

using namespace std;

typedef __uint128_t ui;

#define endl "\n"

const ui N = 18;

constexpr ui pow10(const ui& exp)
{
    if (exp == 0)
        return 1;
    return 10 * pow10(exp - 1);
};

string ui128_to_string(__uint128_t n)
{
    stringstream stream;
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }

    string str = stream.str();
    reverse(str.begin(), str.end());

    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __uint128_t& n)
{
    os << ui128_to_string(n);
    return os;
}

ui pow(ui base, ui exp)
{
    ui r = 1;

    while (exp > 0)
    {
        ui bit = exp % 2;
        if (bit == 1)
            r = r * base;
        exp /= 2;
        base *= base;
    }

    return r;
}

class Polynomial
{
    private:
        unordered_map<ui, ui> coeffs;
    public:
        Polynomial() {}
        void inplace_add(ui exp, ui coeff)
        {
            if (coeffs.find(exp) == coeffs.end())
                coeffs[exp] = 0;
            coeffs[exp] += coeff;
        }
        void inplace_multiply(const Polynomial& rhs)
        {
            unordered_map<ui, ui> new_coeffs;
            for (auto const l: coeffs)
            {
                ui lhs_exp = l.first;
                ui lhs_coeff = l.second;
                for (auto const r: rhs.coeffs)
                {
                    ui rhs_exp = r.first;
                    ui rhs_coeff = r.second;
                    ui exp = lhs_exp + rhs_exp;
                    ui coeff = lhs_coeff * rhs_coeff;
                    if (new_coeffs.find(exp) == new_coeffs.end())
                        new_coeffs[exp] = 0;
                    new_coeffs[exp] += coeff;
                }
            }
            coeffs = new_coeffs;
        }
        vector<ui> get_coeffs()
        {
            vector<ui> v;
            for (auto const p: coeffs)
                v.push_back(p.second);
            return v;
        }
        ui evaluate(ui x) const
        {
            ui r = 1;
            for (auto const p: coeffs)
            {
                ui exp = p.first;
                ui coeff = p.second;
                r *= coeff * pow(x, exp);
            }
            return r;
        }
        string to_string()
        {
            stringstream s;
            for (auto const p: coeffs)
                s << p.second << "x^" << p.first << "+";
            return s.str();
        }
};

vector<ui> splitBy0b00(ui n) // split number by '00' in the binary form
{
    vector<ui> parts;
    ui prev_two_bits = 1;
    ui exp = 0;
    ui curr_val = 0;

    while (n > 0)
    {
        ui bit = n & (0x1);
        prev_two_bits = ((prev_two_bits << 1ULL) & 0x3ULL) | bit;
        if (prev_two_bits == 0)
        {
            if (curr_val > 0)
            {
                parts.push_back(curr_val);
                curr_val = 0;
                exp = 0;
            }
        }
        else
        {
            curr_val = curr_val | (bit << exp);
            exp += 1;
        }
        n >>= 1;
    }

    if (curr_val > 0)
        parts.push_back(curr_val);

    return parts;
}

ui Q_brute_force(ui n)
{
    Polynomial f;
    f.inplace_add(0, 1);
    Polynomial g;
    g.inplace_add(0, 1);
    g.inplace_add(1, 1);
    g.inplace_add(2, 1);
    g.inplace_add(3, 1);
    g.inplace_add(4, 1);
    while (n > 0)
    {
        if (n % 2 == 1)
            f.inplace_multiply(g);
        g.inplace_multiply(g);
        n /= 2;
    }
    vector<ui> coeffs = f.get_coeffs();
    ui count = 0;
    for (auto const coeff: coeffs)
        count += (coeff % 2);
    return count;
}

ui Q(ui n, unordered_map<ui, ui>& cache)
{
    ui ans = 1;

    vector<ui> parts = splitBy0b00(n);
    if (parts.size() == 1)
    {
        if (cache.find(parts[0]) == cache.end())
            cache[parts[0]] = Q_brute_force(parts[0]);
        ans = cache[parts[0]];
        return ans;
    }

    for (auto const next_n: parts)
    {
        if (cache.find(next_n) == cache.end())
        {
            ui Q_next_n = Q(next_n, cache);
            cache[next_n] = Q_next_n;
        }
        ans *= cache[next_n];
    }

    return ans;
}

int main()
{
    ui ans = 0;

    unordered_map<ui, ui> cache;

    cache[1] = 5;

    for (ui exp = 1; exp <= N; exp++)
        ans += Q(pow10(exp), cache);

    cout << ans << endl;

    return 0;
}

