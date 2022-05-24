// Question: https://projecteuler.net/problem=408

/*
  Stategy:
   . Let F(A_1, A_2, ..., A_n) be a function calculating the number of paths from A_1 to A_n avoiding the middle points (A_2, A_3, ..., A_{n-1}).
   . Let g(A_1, A_2) denote a function calculating the number of paths from A_1 to A_2.
        . g(A_1, A_2) is the number of North-East paths in a grid (https://en.wikipedia.org/wiki/Lattice_path#North-East_lattice_paths)
        . g(A_1, A_2) = C(n+k, n) = (n+k)! / n! / k! where n = A_2.x - A_1.x and k = A_2.y - A_1.y
        . Note that g(A_1, A_2) = C(n+k, n) if A_2.x >= A_1.x and A_2.y >= A_1.y
                    g(A_1, A_2) = 0 otherwise.
        . We can use inverse_mod function to find 1/n! if we know the value of n!.
        . We can precalculate the k! mod 10**9+7 for k upto 2n.
   . We have, via the principle of inclusion-exclusion,
        . F(A_1, A_2) = g(A_1, A_2)

        . F(A_1, A_2, A_3) = g(A_1, A_3) - g(A_1, A_2) * g(A_2, A_3)
                           = g(A_1, A_3) - F(A_1, A_2) * g(A_2, A_3)

        . F(A_1, A_2, A_3, A_4) = g(A_1, A_4) - g(A_1, A_2) * g(A_2, A_4) - g(A_1, A_3) * g(A_3, A_4) + g(A_1, A_2) * g(A_2, A_3) * g(A_3, A_4)
                                = g(A_1, A_4) - [g(A_1, A_3) - g(A_1, A_2) * g(A_2, A_3)] * g(A_3, A_4) - g(A_1, A_2) * g(A_2, A_4)
                                = g(A_1, A_4) - F(A_1, A_2, A_3) * g(A_3, A_4) - F(A_1, A_2) * g(A_2, A_4)

        . F(A_1, A_2, A_3, A_4, A_5) = g(A_1, A_5) - g(A_1, A_2) * g(A_2, A_5) - g(A_1, A_3) * g(A_3, A_5) - g(A_1, A_4) * g(A_4, A_5)
                                       + g(A_1, A_2) * g(A_2, A_3) * g(A_3, A_5) + g(A_1, A_2) * g(A_2, A_4) * g(A_4, A_5) + g(A_1, A_3) * g(A_3, A_4) * g(A_4, A_5)
                                       -+ g(A_1, A_2) * g(A_2, A_3) * g(A_3, A_4) * g(A_4, A_5)
                                     = g(A_1, A_5) - g(A_2, A_5) * F(A_1, A_2) - g(A_3, A_5) * F(A_1, A_2, A_3) - g(A_4, A_5) * F(A_1, A_2, A_3, A_4)

    . From the patterns of the F function with different number of points, we can generalize F(A_1, A_2, ..., A_n) as
        . F(A_1, A_2, ..., A_n) = sum_{j=1 to n-1} {g(A_j, A_n) * F(A_1, ..., A_j)}
*/

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<unordered_set>
#include<cmath>
#include<cassert>
#include<algorithm>
#include<unordered_map>
#include<string>
#include<sstream>

using namespace std;

typedef int64_t ui;
#define endl "\n"

const ui N = 10'000'000;
const ui MOD = 1'000'000'007;

vector<ui> factorial_cache(2*N+1);
unordered_map<ui, ui> inv_factorial_cache;

class Point
{
    public:
        ui x;
        ui y;
        Point() { x = y = 0; }
        Point(const ui& x, const ui& y)
        {
            this->x = x;
            this->y = y;
        }
        bool operator < (const Point& rhs) const
        {
            if (this->x == rhs.x)
                return this->y < rhs.y;
            return this->x < rhs.x;
        }
        bool operator == (const Point& rhs) const
        {
            return (this->x == rhs.x) && (this->y == rhs.y);
        }
        bool exists_path_to(const Point& rhs) const
        {
            return (this->x <= rhs.x) && (this->y <= rhs.y);
        }
        string to_string() const
        {
            stringstream s;
            s << "(" << this->x << " " << this->y << ")";
            return s.str();
        }
};

struct PointHashFunctor
{
    ui operator() (const Point& p) const
    {
        return p.x * N + p.y;
    }
};

ui inverse_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

vector<Point> get_inadmissible_points(const ui& n)
{
    unordered_set<ui> considered_square_numbers; // those are all square numbers <= N
    unordered_set<ui> all_square_numbers; // those are all square numbers such that <= 2*N

    ui upperbound = ui(sqrt(n));

    for (ui x = 1; x <= upperbound; x++)
    {
        considered_square_numbers.insert(x*x);
        all_square_numbers.insert(x*x);
    }
    for (ui x = upperbound+1; x <= 2*upperbound; x++)
        all_square_numbers.insert(x*x);

    vector<Point> inadmissible_points;

    for (auto x: considered_square_numbers)
        for (auto y: considered_square_numbers)
            if (all_square_numbers.find(x+y) != all_square_numbers.end())
                inadmissible_points.emplace_back(x, y);

    return inadmissible_points;
}

void initialize_factorial_cache(vector<ui>& factorial_cache)
{
    ui r = 1;
    factorial_cache[0] = 1;
    for (ui i = 1; i <= 2*N; i++)
    {
        r *= i;
        r %= MOD;
        factorial_cache[i] = r;
    }
}

ui count_num_paths(const Point& a, const Point& b)
{
    //cout << "counting: " << a.to_string() << "->" << b.to_string() << endl;
    if (!a.exists_path_to(b))
        return 0;
    ui n = b.x - a.x;
    ui k = b.y - a.y;

    // C(n+k, n) = (n+k)! / n! / k!
    if (inv_factorial_cache.find(n) == inv_factorial_cache.end())
        inv_factorial_cache[n] = inverse_mod(factorial_cache[n], MOD);
    if (inv_factorial_cache.find(k) == inv_factorial_cache.end())
        inv_factorial_cache[k] = inverse_mod(factorial_cache[k], MOD);
    
    ui C = 1;
    C = factorial_cache[n+k] * inv_factorial_cache[n];
    C %= MOD;
    C *= inv_factorial_cache[k];
    C %= MOD;

    return C;
}

int main()
{
    Point source_point(0, 0);
    Point dest_point(N, N);

    vector<Point> inadmissible_points = get_inadmissible_points(N);
    inadmissible_points.push_back(dest_point); // adding the destination point so we don't have to repeat the calculation at the final step

    uint64_t n_points = 0;
    n_points = inadmissible_points.size();
    sort(inadmissible_points.begin(), inadmissible_points.end());

    initialize_factorial_cache(factorial_cache);

    unordered_map<Point, ui, PointHashFunctor> num_paths_from_source_to;
    for (ui curr_point_index = 0; curr_point_index < n_points; curr_point_index++)
    {
        const Point& curr_point = inadmissible_points[curr_point_index];
        ui path_count = 0; // this will count how many paths to this point avoiding the points in the middle
        path_count += count_num_paths(source_point, curr_point);
        for (ui prev_point_index = 0; prev_point_index < curr_point_index; prev_point_index++)
        {
            const Point& prev_point = inadmissible_points[prev_point_index];
            path_count -= num_paths_from_source_to[prev_point] * count_num_paths(prev_point, curr_point);
            path_count %= MOD;
        }
        num_paths_from_source_to[curr_point] = path_count;
    }

    cout << num_paths_from_source_to[dest_point] << endl;

    return 0;
}
