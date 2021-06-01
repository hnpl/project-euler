#include<iostream>
#include<set>
#include<vector>
#include<fstream>
#include<algorithm>
#include<array>
#include<cmath>

using namespace std;

typedef uint32_t ui;

#define endl "\n"

const ui N = 1000000;

vector<ui> get_primes()
{
    vector<ui> primes;

    ifstream input_stream("../inputs/primes_1e6.txt");
    ui n;
    while (input_stream >> n)
        primes.push_back(n);
    input_stream.close();

    return primes;
}

ui get_digit_sum(ui n)
{
    ui ans = n;
    while (ans >= 10)
    {
        ui k = ans;
        ans = 0;
        while (k > 0)
        {
            ans += (k % 10);
            k = k / 10;
        }
    }
    return ans;
}

int main()
{
    ui ans = 0;

    vector<ui> primes = get_primes();
    set<ui> prime_set(primes.begin(), primes.end());

    array<ui, N+1> DRS;
    fill(DRS.begin(), DRS.end(), 0);

    array<ui, N+1> MDRS;
    fill(MDRS.begin(), MDRS.end(), 0);

    for (ui n = 1; n <= N; n++)
        DRS[n] = get_digit_sum(n);

    for (ui n = 1; n <= N; n++)
        MDRS[n] = DRS[n];
    
    for (ui n = 2; n < N; n++)
    {
        if (prime_set.find(n) != prime_set.end())
            continue;
        ui sqrt_n = ui(sqrt(double(n)));
        for (ui d = 2; d <= sqrt_n; d++)
        {
            if (n % d != 0)
                continue;
            MDRS[n] = max(MDRS[n], DRS[d] + MDRS[n/d]);
            //MDRS[n] = max(MDRS[n], DRS[n/d] + MDRS[d]);
        }
    }

    for (ui n = 2; n < N; n++)
        ans += MDRS[n];

    cout << ans << endl;

    return 0;
}