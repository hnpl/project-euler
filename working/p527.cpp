#include <iostream>
#include <iomanip>
#include <unordered_map>

using namespace std;

typedef uint64_t ui;

constexpr ui pow10(ui n)
{
    if (n == 0)
        return 1;
    return pow10(n-1) * 10ULL;
};


double R(ui N)
{
    double Nf = double(N);
    double ans = 0.0;

    double next_R = 0;
    double R_sum = 1.0;

    // R
    for (ui n = 2; n <= N; n++)
    {
	    double nf = double(n);
	    next_R = 1.0 + 2.0 / (nf * nf) * R_sum;
	    ans = 1.0 + 2.0 / (nf * nf) * R_sum;
	    R_sum += n * next_R;
    }

    return ans;
}

double B(ui N, unordered_map<ui, double>& cache)
{
    if (cache.find(N) == cache.end())
    {
        ui m = (N+1)/2;
        double mm = double(m);
        cache[N] = 1.0 + (mm-1)/N * B(m-1, cache) + (N-mm)/N * B(N-m, cache);
    }
    return cache[N];
}

int main()
{
    const ui N = pow10(10);

    unordered_map<ui, double> B_cache;
    B_cache[0] = 0;
    B_cache[1] = 1;

    double ans = R(N) - B(N, B_cache);

    cout << setprecision(12) << ans << endl;
    return 0;
}
