// Problem: https://projecteuler.net/problem=136

/*
    Let x = a + 2b
        y = a +  b
        z = a
    So, x^2 - y^2 - z^2 = n
     => (a+b) * (-a + 3b) = n
    Let X = a+b, Y = -a+3b, so n=XY and X > 0, which means Y > 0, and X <= n and Y <= n.
    We can iterate through all possible X and Y and count the number of times XY appears.
    Note that a and b are integers, so we need to check whether (X,Y) produces integers a and b.
    We have, b = (X+Y)/4 and a = (3X-Y)/4.
    Therefore, X+Y must be divisible by 4, and 3X-Y must be divisible by 4, and X+Y > 0 and 3X-Y > 0.
*/

#include<iostream>
#include<unordered_map>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 50000000;
const ui M = 1;

int main()
{
    ui ans = 0;

    unordered_map<ui, ui> freq;

    for (ui X = 1; X < N; X++)
    {
        for (ui Y = 1; Y < 3*X; Y++)
        {
            if (X*Y > N)
                break;
            if (((X+Y)%4 == 0) & ((3*X-Y)%4==0))
            {
                if (freq.find(X*Y) == freq.end())
                    freq[X*Y] = 0;
                freq[X*Y] += 1;
            }
        }
    }

    for (auto p: freq)
        if (p.second == 1)
            ans = ans + 1;

    cout << ans << endl;

    return 0; 
}
