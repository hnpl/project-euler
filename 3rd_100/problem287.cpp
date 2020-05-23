// Question: https://projecteuler.net/problem=287

// Python version is more than 100 times slower -> it's worth it to use C++

// g++ problem287.cpp -O3 -std=c++17

#include<iostream>
#include<stdio.h>
#include<cmath>
#include<vector>
#include<tuple>

using namespace std;

typedef int64_t int64;
typedef tuple<int64, int64, int64, int64> rectangle;

const int64 N = 24;
const int64 alpha_ = pow(2, N-1);
const int64 beta_ = pow(2, 2*N - 2);

// The image is a filled circle

uint inside_circle(int64 x, int64 y)
{
    if ((x-alpha_)*(x-alpha_) + (y-alpha_)*(y-alpha_) - beta_ <= 0)
        return 1;
    return 0;
}

bool different_halves(int64 p1, int64 p2)
{
    return (p1 <= alpha_ - 1) != (p2 <= alpha_ - 1);
}

vector<rectangle> get_quarters(rectangle r)
{
    auto [x1, y1, x2, y2] = r;

    vector<rectangle> quarters;
    // quarter 1 -> top left
    // quarter 2 -> top right
    // quarter 3 -> bottom left
    // quarter 4 -> bottom right
    auto mid_x = (x1 + x2) / 2;
    auto mid_y = (y1 + y2) / 2;

    quarters.push_back(make_tuple(       x1, mid_y + 1, mid_x, y2)); 
    quarters.push_back(make_tuple(mid_x + 1, mid_y + 1,    x2, y2));
    quarters.push_back(make_tuple(       x1,        y1, mid_x, mid_y));
    quarters.push_back(make_tuple(mid_x + 1,        y1,    x2, mid_y));

    return quarters;
}

bool same_color(rectangle r)
{
    auto [x1, y1, x2, y2] = r;

    // There are 3 cases:
    // Case 1 -> all points are inside the circle
    // Case 2 -> if all points are outside the circle, then the rectangle has one color if either (x1 and x2) are in same half of the circle and (y1 and y2) are in same half of the circle
    // Case 3 -> the subregion has 2 colors otherwise
    int64 n_insides = inside_circle(x1, y1) + inside_circle(x1, y2) + inside_circle(x2, y1) + inside_circle(x2, y2);
    return (n_insides == 4) || ((n_insides == 0) && !different_halves(x1, x2) && !different_halves(y1, y2));
}

int64 compressed_length(rectangle r)
{
    auto [x1, y1, x2, y2] = r;

    if (((x1 == x2) || (y1 == y2)) || same_color(r))
        return 2;

    int64 mid_x = (x1 + x2) / 2;
    int64 mid_y = (y1 + y2) / 2;

    int64 length = 1;
    auto quarters = get_quarters(r);
    for (auto quarter: quarters)
        length = length + compressed_length(quarter);
    return length;
}

int main()
{
    auto ans = compressed_length(make_tuple(0, 0, pow(2, N) - 1, pow(2, N) - 1));
    cout << ans;
    return 0;
}
