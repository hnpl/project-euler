// Question: https://projecteuler.net/problem=259

// g++ problem259.cpp -O3 -std=c++17

#include<algorithm>
#include<vector>
#include<iostream>
#include<numeric>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<utility>

using namespace std;

#define endl "\n"

struct Fraction
{
    int64_t numerator, denominator;
    Fraction()
    {
        numerator = 0;
        denominator = 1;
    }
    Fraction(int64_t x, int64_t y)
    {
        numerator = 0;
        denominator = 1;
        if (x == 0) // 0 = 0/1
            return;
        int64_t GCD = gcd(x, y);
        if (y < 0) { x = -x; y = -y; }
        numerator = x / GCD;
        denominator = y / GCD;
    }
};

inline bool operator==(const Fraction& lhs, const int64_t& rhs)
{
    return (lhs.numerator == rhs) && (lhs.denominator == 1);
}

inline bool operator>(const Fraction& lhs, const int64_t& rhs)
{
    int64_t rhs_d = rhs * lhs.denominator; // denominator > 0
    return lhs.numerator > rhs_d;
}

inline bool operator<(const Fraction& lhs, const int64_t& rhs)
{
    int64_t rhs_d = rhs * lhs.denominator; // denominator > 0
    return lhs.numerator < rhs_d;
}

inline bool operator==(const Fraction& lhs, const Fraction& rhs)
{
    return (lhs.numerator == rhs.numerator) && (lhs.denominator == rhs.denominator);
}

inline Fraction operator+(const Fraction& lhs, const Fraction& rhs)
{
    int64_t lcm = lhs.denominator * rhs.denominator / gcd(lhs.denominator, rhs.denominator);
    return Fraction(lhs.numerator * (lcm / lhs.denominator) + rhs.numerator * (lcm / rhs.denominator), lcm);
}

inline Fraction operator-(const Fraction& lhs, const Fraction& rhs)
{   
    int64_t lcm = lhs.denominator * rhs.denominator / gcd(lhs.denominator, rhs.denominator);
    return Fraction(lhs.numerator * (lcm / lhs.denominator) - rhs.numerator * (lcm / rhs.denominator), lcm);
}

inline Fraction operator*(const Fraction& lhs, const Fraction& rhs)
{
    return Fraction(lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator);
}

inline Fraction operator/(const Fraction& lhs, const Fraction& rhs)
{
    return Fraction(lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator);
}

struct FractionHash
{
    size_t operator() (const Fraction &f) const
    {
        return f.numerator ^ f.denominator;
    }
};

const int64_t N = 9;

unordered_map<string, unordered_set<Fraction, FractionHash>> cached_dfs;

unordered_set<Fraction, FractionHash> dfs(const string& pattern)
{
    unordered_set<Fraction, FractionHash> reached_fractions;
    int64_t n_digits = pattern.size();
    if (cached_dfs.find(pattern) != cached_dfs.end())
        return cached_dfs[pattern];

    reached_fractions.insert(Fraction(atoi(pattern.c_str()),1));

    for (int64_t middle = 1; middle < n_digits; middle++)
    {
        string left_pattern = pattern.substr(0, middle);
        string right_pattern = pattern.substr(middle, n_digits-middle);
        
        auto left_fractions = dfs(left_pattern);
        auto right_fractions = dfs(right_pattern);
        for (auto left_fraction: left_fractions)
        {
            for (auto right_fraction: right_fractions)
            {
                reached_fractions.insert(left_fraction + right_fraction);
                reached_fractions.insert(left_fraction - right_fraction);
                reached_fractions.insert(left_fraction * right_fraction);
                if (right_fraction.numerator != 0)
                    reached_fractions.insert(left_fraction / right_fraction);
            }
        }
    }
    cached_dfs[pattern] = move(reached_fractions);
    cout << pattern << " " << cached_dfs[pattern].size() << endl;

    return cached_dfs[pattern];
}

int64_t sum(const string& pattern)
{
    int64_t ans = 0;

    auto reached_fractions = dfs(pattern);
    for (auto fraction: reached_fractions)
        if (fraction.denominator == 1 && fraction.numerator > 0)
            ans += fraction.numera;

    return ans;
}

int main()
{
    int64_t ans = sum("123456789");
    cout << ans << endl;
    return 0;
}
