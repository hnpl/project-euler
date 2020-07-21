// Problem: https://projecteuler.net/problem=504

// g++ problem504.cpp --std=c++17

#include<iostream>
#include<numeric>
#include<unordered_set>
#include<vector>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui M = 100;

inline ui count_interior_points(ui a, ui b)
{
    /*
        Calculate the area of the rectangle:
            alpha = a*b
        Calculate the number of lattice points on the sides of the rectangle:
            beta = 2*(a+2) + 2*(b+2) - 4
        Calculate the number of lattice points on ONE diagonal of the rectangle:
            gammma = a/(b/gcd(a,b))+1
        Calculate the number of interior points of the triangle:
            [alpha - beta - (gamma - 2)] / 2
    */
    ui alpha = (a+1)*(b+1);
    ui beta = 2*(a+b);
    ui gamma = b/(b/gcd(a,b))+1;

    return (alpha - beta - gamma + 2) / 2;
}

inline ui key(ui a, ui b)
{
    return  a*(M+1) + b;
}

int main()
{
    ui ans = 0;

    vector<ui> n_interior_points = vector<ui>((M+2)*M);
    
    unordered_set<ui> squares;
    
    for (ui a = 1; a <= M; a++)
        for (ui b = a; b <= M; b++)
        {
            auto n_points = count_interior_points(a, b);
            n_interior_points[key(a, b)] = n_points;
            n_interior_points[key(b, a)] = n_points;
        }
    
    for (ui i = 1; i <= 4*M*M; i++)
        squares.insert(i*i);

    for (ui a = 1; a <= M; a++)
        for (ui b = 1; b <= M; b++)
            for (ui c = 1; c <= M; c++)
                for (ui d = 1; d <= M; d++)
                {
                    ui n_interiors = n_interior_points[key(a, b)] \
                                   + n_interior_points[key(b, c)] \
                                   + n_interior_points[key(c, d)] \
                                   + n_interior_points[key(d, a)] \
                                   + a + b + c + d
                                   - 3;
                    if (squares.find(n_interiors) != squares.end())
                        ans = ans + 1;
                }

    cout << ans << endl;

    return 0;
}
