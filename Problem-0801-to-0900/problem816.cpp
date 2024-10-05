// Question: https://projecteuler.net/problem=816

// Split the plane into a grid of nxn cells. This effectively performs a radix sort on the points coordinates.
// Choose a cell size so that most cells are empty or only have 1 point.
// Iterate through non-empty cells, calculate the distance between the point in this cell to the points in its neighbor cells.

#include<iostream>
#include<cstdint>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<utility>
#include<limits>
#include<iomanip>

typedef int64_t ui;
const ui N = 2000000;
const ui SEED = 290797;
const ui MOD = 50515093;
const ui UI_MAX = 1ULL << 62;

using namespace std;

class Point
{
    public:
        ui x, y;
        Point()
        {
            this->x = 0;
            this->y = 0;
        }
        Point(const ui& x, const ui& y)
        {
            this->x = x;
            this->y = y;
        }
        double distance(const Point& other) const
        {
            const double delta_x = this->x - other.x;
            const double delta_y = this->y - other.y;
            return sqrt(delta_x * delta_x + delta_y * delta_y);
        }
        ui distance_square(const Point& other) const
        {
            const ui delta_x = this->x - other.x;
            const ui delta_y = this->y - other.y;
            return delta_x * delta_x + delta_y * delta_y;
        }
        bool operator== (const Point& rhs) const
        {
            return (this->x == rhs.x) && (this->y == rhs.y);
        }
};

class RNG
{
    public:
        ui curr;
        ui mod;
        RNG(const ui& seed, const ui& mod)
        {
            this->curr = seed;
            this->mod = mod;
        }
        ui get_next()
        {
            ui next = this->curr;
            this->curr = (this->curr) * (this->curr) % (this->mod);
            return next;
        }
};

/*
vector<Point> generate_points(const ui& numPoints)
{
    vector<Point> points;
    points.reserve(numPoints);
    RNG rng(SEED, MOD);
    for (ui i = 0; i < numPoints; i++)
        points.emplace_back(rng.get_next(), rng.get_next());
    return points;
}
*/

const ui hash_offset = 1ULL << 26;

inline ui hash_function(const ui& grid_x, const ui& grid_y)
{
    return grid_x * hash_offset + grid_y;
}

inline void dehash_function(const ui& hash, ui& grid_x, ui& grid_y)
{
    grid_x = hash / (hash_offset);
    grid_y = hash % (hash_offset);
}

unordered_map<ui, vector<Point>> generate_point_grid(const ui& numPoints)
{
    unordered_map<ui, vector<Point>> grid;
    const ui grid_size = 1ULL << 17;
    RNG rng(SEED, MOD);

    for (ui i = 0; i < numPoints; i++)
    {
        ui point_x = rng.get_next();
        ui point_y = rng.get_next();
        const ui grid_x = point_x / grid_size;
        const ui grid_y = point_y / grid_size;
        auto key = hash_function(grid_x, grid_y);
        grid[key].emplace_back(point_x, point_y);
    }

    return grid;
}

ui min_distance_1_vector(const vector<Point>& v)
{
    ui ans = UI_MAX;

    const ui n = v.size();
    for (ui i = 0; i < n; i++)
        for (ui j = i+1; j < n; j++)
            ans = min(ans, v[i].distance_square(v[j]));

    return ans;
}

ui min_distance_2_vectors(const vector<Point>& v1, const vector<Point>& v2)
{
    ui ans = UI_MAX;
    //for (auto const& p1: v1)
    //    for (auto const& p2: v2)
    //        ans = min(ans, p1.distance_square(p2));
    const ui n = v1.size();
    const ui m = v2.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            ans = min(ans, v1[i].distance_square(v2[j]));
    return ans;
}

double solve(const ui& numPoints)
{
    ui ans = 0;

    auto grid = generate_point_grid(numPoints);

    cout << "Non empty grid cells: " << grid.size() << endl;

    // iterate all cells in the grid
    ans = UI_MAX;
    for (auto const& cell: grid)
    {
        ui grid_x;
        ui grid_y;
        dehash_function(cell.first, grid_x, grid_y);
        const vector<Point>& points = cell.second;
        // find the min distance among all pairs in a cell
        ans = min(ans, min_distance_1_vector(points));
        // find the min distance among points in this cell and points in the neighbor cells
        for (int64_t delta_x = 0; delta_x <= 1; delta_x++)
        {
            for (int64_t delta_y = 0; delta_y <= 1; delta_y++)
            {
                if (delta_x == 0 && delta_y == 0)
                    continue;
                ui neighbor_key = hash_function(grid_x + delta_x, grid_y + delta_y);
                auto neighbor = grid.find(neighbor_key);
                if (neighbor == grid.end())
                    continue;
                ans = min(ans, min_distance_2_vectors(points, neighbor->second));
            }
        }
    }

    return sqrt(ans);
}

int main()
{
    double ans = 0;
    ans = solve(N);
    cout << setprecision(12) <<  ans << endl;
    return 0;
}
