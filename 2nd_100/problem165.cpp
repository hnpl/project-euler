// Question: https://projecteuler.net/problem=165

// g++ problem165.cpp -O3 -std=c++17

// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line

#include<algorithm>
#include<vector>
#include<iostream>
#include<numeric>
#include<string>
#include<unordered_set>
#include<utility>

using namespace std;

#define endl "\n"

template<typename T>
struct Point2D
{
    T x, y;
    Point2D() { }
    Point2D(T px, T py) { x = px; y = py; }
};

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

inline bool operator==(const Point2D<Fraction>& lhs, const Point2D<Fraction>& rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

struct Point2DFractionHash
{
    size_t operator() (const Point2D<Fraction> &p) const 
    {
        //return (p.x.numerator ^ p.y.numerator << 32) ^ (p.x.denominator ^ p.y.denominator);
        return p.x.numerator ^ p.y.numerator;
    }
};

struct Segment2D
{
    Point2D<int64_t> p1, p2;
    Segment2D()
    {
        p1.x = p1.y = p2.x = p2.y = 0;
    }
    Segment2D(const Point2D<int64_t>& a, const Point2D<int64_t>& b)
    {
        p1 = a;
        p2 = b;
    }

    pair<Point2D<Fraction>, bool> intersection(const Segment2D& line2)
    {
        Point2D<Fraction> p;

        auto x1 = p1.x;
        auto y1 = p1.y;
        auto x2 = p2.x;
        auto y2 = p2.y;
        auto x3 = line2.p1.x;
        auto y3 = line2.p1.y;
        auto x4 = line2.p2.x;
        auto y4 = line2.p2.y;

        int64_t denominator = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
        if (denominator == 0)
            return make_pair(p, false);

        int64_t numerator_x = (x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4);
        int64_t numerator_y = (x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4);

        p.x = Fraction(numerator_x, denominator);
        p.y = Fraction(numerator_y, denominator);

        return make_pair(p, true);
    }

    bool point_inside_segment(const Point2D<Fraction>& p) // Assuming the point is on the same line as the segment
    {
        int64_t min_x = min(p1.x, p2.x);
        int64_t max_x = p1.x + p2.x - min_x;
        int64_t min_y = min(p1.y, p2.y);
        int64_t max_y = p1.y + p2.y - min_y;
        
        /*
            If the segment is parallel to the x-axis, then min_y == max_y -> p.y == min_y and p.x of the point must be in between min_x and max_x. 
            If the segment is parallel to the y-axis, then min_x == max_x -> p.x == min_x and p.y of the point must be in between min_y and max_y.
            Otherwise, min_x <= p.x <= max_x and min_y <= p.y < max_y
        */
        return (p.x > min_x && p.x < max_x) || (p.y > min_y && p.y < max_y);
    }
};

struct RNG
{
    int64_t s_i = 290797;
    RNG() { s_i = 290797; }
    int64_t next()
    {
        s_i = (s_i * s_i) % 50515093;
        return s_i % 500;
    }
};

const int64_t N = 20000;

int main()
{
    RNG rng;
    vector<int64_t> t;
    t.reserve(N+1);
    for (int64_t i = 0; i < N; i++)
        t.push_back(rng.next());

    vector<Segment2D> segments;
    segments.reserve(N/4);
    auto it = t.begin();
    for (int64_t i=0; i<N; i+=4)
        segments.push_back(Segment2D(Point2D<int64_t>(t[i], t[i+1]), Point2D<int64_t>(t[i+2], t[i+3])));
    auto n_segments = segments.size();

    unordered_set<Point2D<Fraction>, Point2DFractionHash> intersections;

    for (int64_t i = 0; i < n_segments; i++)
    {
        for (int64_t j = i+1; j < n_segments; j++)
        {
            auto segment1 = segments[i];
            auto segment2 = segments[j];
            
            auto intersection = segment1.intersection(segment2);

            if (intersection.second)
                if ((segment1.point_inside_segment(intersection.first)) && (segment2.point_inside_segment(intersection.first)))
                    intersections.insert(intersection.first);
        }
    }

    cout << intersections.size() << endl;

    return 0;
}
