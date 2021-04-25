#include <iostream>
#include <cmath>
#include <iomanip>

#define endl "\n"

typedef uint64_t ui;

using namespace std;

const ui N = 10000000000;
const ui M = 4000;

inline double p(ui k)
{
    return double(-2*k*k + 2*k + 2*N*k - 1)/double(N*N);
}

int main()
{
    double ans = 0.0;
    for (ui k = 1; k <= N/2; k++)
    {
        if (k % 10000000 == 0)
            cout << setprecision(12) << k << " " << ans << endl;
        ans += pow((2*p(k)-1), M);
    }

    cout << setprecision(12) << ans << endl;
}
