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

vector<Point> generate_points(const ui& numPoints)
{
    vector<Point> points;
    points.reserve(numPoints);
    RNG rng(SEED, MOD);
    for (ui i = 0; i < numPoints; i++)
        points.emplace_back(rng.get_next(), rng.get_next());
    return points;
}

double min_distance_1_vector(const vector<Point>& v)
{
    double ans = std::numeric_limits<double>::max();

    const ui n = v.size();
    for (ui i = 0; i < n; i++)
        for (ui j = i+1; j < n; j++)
            ans = min(ans, v[i].distance(v[j]));

    return ans;
}

double min_distance_2_vectors(const vector<Point>& v1, const vector<Point>& v2)
{
    double ans = std::numeric_limits<double>::max();
    for (auto const& p1: v1)
        for (auto const& p2: v2)
            ans = min(ans, p1.distance(p2));
    return ans;
}

double solve(const vector<Point>& points)
{
    double ans = 0;

    ui max_x = 0;
    ui max_y = 0;
    ui min_x = -1;
    ui min_y = -1;
    for (auto const& point: points)
    {
        max_x = max(point.x, max_x);
        max_y = max(point.y, max_y);
        min_x = min(point.x, min_x);
        min_y = min(point.y, min_y);
    }

    const ui grid_size = (1ULL << 10);

    const ui max_grid_x = (max_x+grid_size-1) / grid_size;
    const ui max_grid_y = (max_y+grid_size-1) / grid_size;
    unordered_map<ui, vector<Point>> grid;
    auto hash_function = [max_grid_y](ui grid_x, ui grid_y) -> ui {
        return grid_x * (max_grid_y+1) + grid_y;
    };
    auto dehash_function = [max_grid_y](ui hash) -> pair<ui, ui> {
        return make_pair(hash / (max_grid_y+1), hash % (max_grid_y+1));
    };
    for (auto const& point: points)
    {
        const ui grid_x = point.x / grid_size;
        const ui grid_y = point.y / grid_size;
        auto key = hash_function(grid_x, grid_y);
        if (grid.find(key) == grid.end())
            grid[key] = vector<Point>();
        grid[key].push_back(move(point));
    }

    cout << "Non empty grid cells: " << grid.size() << endl;

    // iterate all cells in the grid
    ans = std::numeric_limits<double>::max();
    for (auto const& cell: grid)
    {
        auto grid_coordinate = dehash_function(cell.first);
        ui grid_x = grid_coordinate.first;
        ui grid_y = grid_coordinate.second;
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

    return ans;
}

int main()
{
    double ans = 0;
    vector<Point> points = generate_points(N);
    ans = solve(points);
    cout << setprecision(12) <<  ans << endl;
    return 0;
}
