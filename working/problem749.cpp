#include <iostream>
#include <utility>
#include <array>
#include <sstream>

using namespace std;

typedef int64_t i64;

#define endl "\n"

const i64 N = 16;
const i64 M = N + 1;

const i64 max_n_digits = 17; // can't have more than 17 digits because 18 * (9^16) < 10^17 (the right hand side grows faster)

i64 n_checks = 0;

constexpr i64 pow(i64 base, i64 exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const i64 dont_know = pow(10, max_n_digits);

constexpr array<i64, M> make_cached_pow10()
{
    array<i64, M> cache = {};
    for (i64 i = 0; i <= N; i++)
        cache[i] = pow(10, i);
    return cache;
};

class Bounds
{
    private:
        i64 lowerbound;
        i64 upperbound;
    public:
        Bounds()
        {
            this->lowerbound = -1;
            this->upperbound = -1;
        }
        Bounds(i64 l, i64 u)
        {
            this->lowerbound = l;
            this->upperbound = u;
        }
        const bool is_valid()
        {
            return this->lowerbound <= this->upperbound; 
        }
        const Bounds intersects(const Bounds& rhs)
        {
            return Bounds(max(this->lowerbound, rhs.lowerbound), min(this->upperbound, rhs.upperbound));
        }
        string to_string()
        {
            stringstream s;
            s << "[" << this->lowerbound <<", " << this->upperbound << "]";
            return s.str();
        }
};

array<i64, 10> num_to_freq(const i64& input)
{
    array<i64, 10> freq{0,0,0,0,0,0,0,0,0,0};
    i64 n = input;
    while (n > 0)
    {
        freq[n%10] += 1;
        n = n / 10;
    }
    return freq;
}

void dfs(i64 depth, array<i64, 10>& freq, const i64& prev_digit, const i64& curr_lhs_sum, const i64& exp,
         const Bounds& target_bounds, i64& ans)
{

    if (depth > 1)
    {
        // case 1
        i64 n = curr_lhs_sum - 1;
        auto rhs_freq = num_to_freq(n);
        if (rhs_freq == freq)
        {
            ans += n;
            cout << n << endl;
            return;
        }
        // case 2
        n = curr_lhs_sum + 1;
        rhs_freq = num_to_freq(n);
        if (rhs_freq == freq)
        {
            ans += n;
            cout << n << endl;
            return;
        }
    }

    for (i64 next_digit = prev_digit; next_digit <= 9; next_digit++)
    {
        freq[next_digit] += 1;

        i64 next_lhs_sum = curr_lhs_sum + pow(next_digit, exp);
        auto lhs_bounds = Bounds(next_lhs_sum, next_lhs_sum + (max_n_digits - depth - 1) * pow(9, exp)); // lhs: digit power sum
        auto rhs_bounds = Bounds(pow(10, depth), dont_know);                                             // rhs: n
        auto new_bounds = lhs_bounds.intersects(rhs_bounds);
        new_bounds = new_bounds.intersects(target_bounds);

        if (!new_bounds.is_valid())
        {
            freq[next_digit] -= 1;
            continue;
        }

        dfs(depth+1, freq, next_digit, next_lhs_sum, exp, new_bounds, ans);

        freq[next_digit] -= 1;
    }
}


int main()
{
    i64 ans = 0;
    array<i64, 10> freq {0,0,0,0,0,0,0,0,0,0};
    for (i64 k = 2; k <= N; k++)
    {
        Bounds target_bounds(0, max_n_digits * pow(9, k));
        dfs(0, freq, 0, 0, k, target_bounds, ans);
    }
    cout << ans << endl;
    return 0;
}
