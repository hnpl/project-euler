// Question: https://projecteuler.net/problem=155

/*
    - To form all combinations using N capacitors,
        for all A and B such that A+B=N, we can combine all combinations of A capacitors and all combinations of B capacitors in parallel or in series.
    - Use fractions to store the distinct capacitance of capacitors.
*/

#include<iostream>
#include<numeric>
#include<vector>
#include<unordered_set>
#include<unordered_map>

using namespace std;

typedef uint32_t ui;

#define endl "\n"

const ui N = 18;
const ui C = 1; // it doesn't matter whether C = 1 or C = 60, we can always factor C or C^2 out

/*
unordered_map<ui, unordered_map<ui, ui>> gcd_cache;

ui cached_gcd(ui x, ui y)
{
    if (x > y)
        swap(x, y);
    if (gcd_cache.find(x) == gcd_cache.end())
        gcd_cache[x] = unordered_map<ui, ui>();
    if (gcd_cache[x].find(y) == gcd_cache[x].end())
        gcd_cache[x][y] = gcd(x, y);
    return gcd_cache[x][y];
}
*/

class Fraction
{
    public:
        ui numerator, denominator;
        Fraction()
        {
            numerator = 0;
            denominator = 1;
        }
        Fraction(ui x, ui y)
        {
            numerator = 0;
            denominator = 1;
            if (x == 0) // 0 = 0/1
                return;
            ui GCD = gcd(x, y);
            if (y < 0) { x = -x; y = -y; }
            numerator = x / GCD;
            denominator = y / GCD;
        }
};

inline Fraction series(const Fraction& c1, const Fraction& c2)
{
    return Fraction(c1.numerator * c2.denominator + c1.denominator * c2.numerator, c1.denominator * c2.denominator);
}

inline Fraction parallel(const Fraction& c1, const Fraction& c2)
{
    return Fraction(c1.numerator * c2.numerator, c1.numerator * c2.denominator + c1.denominator * c2.numerator);
}

inline bool operator==(const Fraction& lhs, const ui& rhs)
{
    return (lhs.numerator == rhs) && (lhs.denominator == 1);
}

inline bool operator>(const Fraction& lhs, const ui& rhs)
{
    int64_t rhs_d = rhs * lhs.denominator; // denominator > 0
    return lhs.numerator > rhs_d;
}

inline bool operator<(const Fraction& lhs, const ui& rhs)
{
    int64_t rhs_d = rhs * lhs.denominator; // denominator > 0
    return lhs.numerator < rhs_d;
}

inline bool operator==(const Fraction& lhs, const Fraction& rhs)
{
    return (lhs.numerator == rhs.numerator) && (lhs.denominator == rhs.denominator);
}

struct FractionHash
{
    size_t operator() (const Fraction &p) const 
    {
        return (p.numerator << 16) ^ p.denominator;
    }
};

typedef unordered_set<Fraction, FractionHash> FractionSet;

vector<FractionSet> DP = vector<FractionSet>(N+1);

void do_DP(ui n)
{
    for (ui i = 1; i <= n / 2; i++)
    {
        ui j = n - i;
        for (const auto p1: DP[i])
        {
            for (const auto p2: DP[j])
            {
                Fraction v1 = series(p1, p2);
                Fraction v2 = parallel(p1, p2);
                DP[n].insert(v1);
                DP[n].insert(v2);
            }
        }
    }
}

int main()
{
    ui ans = 0;

    DP[1].insert(Fraction(C, 1));

    for (ui n = 2; n <= N; n++)
    {
        do_DP(n);
        cout << n << " " << DP[n].size() << endl;
    }

    FractionSet agg;
    for (auto f_set: DP)
    {
        for (auto fraction: f_set)
            agg.insert(fraction);
    }

    ans = agg.size();

    cout << ans << endl;

    return 0;
}