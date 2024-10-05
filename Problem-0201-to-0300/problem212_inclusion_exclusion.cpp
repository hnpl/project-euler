// Question: https://projecteuler.net/problem=212

// Something I learned after reading the problem thread.

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
    Cuboid intersect(Cuboid other)
    {
        Cuboid intersection = Cuboid();
        Z64 x2 = x + dx;
        Z64 y2 = y + dy;
        Z64 z2 = z + dz;
        Z64 other_x2 = other.x + other.dx;
        Z64 other_y2 = other.y + other.dy;
        Z64 other_z2 = other.z + other.dz;

        if (x2 <= other.x || other_x2 <= x || y2 <= other.y || other_y2 <= y || z2 <= other.z || other_z2 <= z)
            return intersection;

        intersection.x = max(x, other.x);
        intersection.y = max(y, other.y);
        intersection.z = max(z, other.z);
        intersection.dx = min(x2, other_x2) - intersection.x;
        intersection.dy = min(y2, other_y2) - intersection.y;
        intersection.dz = min(z2, other_z2) - intersection.z;

        return intersection;
    }
    Z64 volume()
    {
        return dx * dy * dz;
    }
    bool is_valid()
    {
        return ((dx > 0) && (dy > 0) && (dz > 0));
    }
};

int main()
{
    Z64 ans = 0;

    vector<Cuboid> cuboids;
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
        cuboids.push_back(c);
    }

    vector<Cuboid> odd; // the number of overlapped cubes of the intersection is odd
    vector<Cuboid> even;

    for (auto c: cuboids)
    {
        auto n_odd = odd.size();
        auto n_even = even.size();

        odd.push_back(c);

        for (auto it = odd.begin(); it != (odd.begin() + n_odd); it++)
        {
            auto c_intersection = c.intersect(*it);
            if (c_intersection.is_valid())
                even.push_back(c_intersection);
        }

        for (auto it = even.begin(); it != (even.begin() + n_even); it++)
        {
            auto c_intersection = c.intersect(*it);
            if (c_intersection.is_valid())
                odd.push_back(c_intersection);
        }
    }

    for (auto c: odd)
        ans = ans + c.volume();
    for (auto c: even)
        ans = ans - c.volume();

    cout << ans << endl;

    return 0;
}