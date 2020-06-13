// Question: https://projecteuler.net/problem=212

#include<iostream>
#include<vector>
#include<unordered_set>
#include<string>
#include<sstream>
#include<algorithm>
#include<utility>

using namespace std;

#define endl "\n"

typedef int64_t Z64;

const Z64 N = 50000;


struct Cuboid
{
    Z64 x, y, z, dx, dy, dz;
    Cuboid() { x = y = z = dx = dy = dz = 0; }
    Cuboid(Z64 a, Z64 b, Z64 c, Z64 d, Z64 e, Z64 f)
    {
        x = a; y = b; z = c;
        dx = d; dy = e; dz = f;
    }
    Z64 has_z_cut(Z64 what_z)
    {
        return (what_z >= z) && (what_z < (z+dz));
    }
};

struct CuboidHash
{
    size_t operator()(const Cuboid& c) const
    {
        return (c.x ^ c.y ^ c.z);
    }
};

inline bool operator==(const Cuboid& lhs, const Cuboid& rhs)
{
    return (lhs.x == rhs.x) &&
           (lhs.y == rhs.y) &&
           (lhs.z == rhs.z) &&
           (lhs.dx == rhs.dx) &&
           (lhs.dy == rhs.dy) &&
           (lhs.dz == rhs.dz);
}

struct Rectangle 
{ 
    Z64 x, y, dx, dy; 
    Rectangle(Z64 a, Z64 b, Z64 c, Z64 d) 
    { x = a; y = b; dx = c; dy = d; }
};

bool points_of_segments_compare(const pair<int, bool>& lhs, const pair<int, bool>& rhs)
{
    return lhs.first < rhs.first;
}

struct Grid
{
    vector<Rectangle> rectangles;
    Z64 min_x;
    Z64 max_x;
    Z64 min_y;
    Z64 max_y;

    Grid() {}
    void reset() { rectangles.clear(); }
    void add(const Cuboid& c) 
    { 
        rectangles.push_back(Rectangle(c.x, c.y, c.dx, c.dy)); 
        min_x = min(min_x, c.x);
        max_x = max(max_x, c.x + c.dx);
        min_y = min(min_y, c.y);
        max_y = max(max_y, c.y + c.dy);
    }
    // https://www.jstor.org/stable/2318871
    Z64 segments_length(vector<pair<Z64, bool>>& points)
    { //points: [(start_point1, false), (end_point1, true), (start_point2, false), (end_point2, true), ...]
        Z64 n = points.size();
        Z64 ans = 0;
        sort(points.begin(), points.end(), points_of_segments_compare);
        Z64 alt = 0;
        for (Z64 i = 0; i < n; i++)
        {
            if (alt != 0)
                ans = ans + (points[i].first - points[i-1].first);
            if (points[i].second)
                alt = alt - 1;
            else
                alt = alt + 1;
        }
        return ans;
    }
    Z64 get_area()
    {
        Z64 area = 0;

        for (Z64 x = min_x; x < max_x; x++)
        {
            vector<pair<Z64, bool>> points;
            for (auto r: rectangles)
            {
                if (r.x <= x && (r.x+r.dx) > x)
                {
                    points.push_back(make_pair(r.y, false));
                    points.push_back(make_pair(r.y + r.dy, true));
                }
            }
            area = area + segments_length(points);
        }

        return area;
    }
};

int main()
{
    Z64 ans = 0;

    unordered_set<Cuboid, CuboidHash> cuboids;
    vector<Z64> seq;
    seq.reserve(300000);
    for (Z64 k = 1; k <= 55; k++)
        seq.push_back((100003 - 200003*k + 300007*k*k*k) % 1000000);
    for (Z64 k = 56; k <= 300000; k++)
        seq.push_back((seq[k-25] + seq[k-56]) % 1000000);
    for (Z64 k = 0; k < N*6; k+=6)
    {
        Cuboid c = Cuboid(seq[k] % 10000, seq[k+1] % 10000, seq[k+2] % 10000,
                          1 + seq[k+3] % 399, 1 + seq[k+4] % 399, 1 + seq[k+5] % 399);
        cuboids.insert(c);
    }

    Z64 min_z = 20000;
    Z64 max_z = 0;
    Z64 min_x = 20000;
    Z64 max_x = 0;
    Z64 min_y = 20000;
    Z64 max_y = 0;
    for (auto c: cuboids)
    {
        min_z = min(min_z, c.z);
        max_z = max(max_z, c.z + c.dz);
        min_x = min(min_x, c.x);
        max_x = max(max_x, c.x + c.dx);
        min_y = min(min_y, c.y);
        max_y = max(max_y, c.y + c.dy);
    }

    Grid z_cut;

    for (Z64 z = min_z; z <= max_z; z++)
    {
        z_cut.reset();
        for (auto c: cuboids)
            if (c.has_z_cut(z))
                z_cut.add(c);
        for (auto it = cuboids.begin(); it != cuboids.end(); )
        {
            if (it->z + it->dz <= z)
                it = cuboids.erase(it);
            else
                it++;
        }
        ans = ans + z_cut.get_area();
    }

    cout << ans << endl;

    return 0;
}