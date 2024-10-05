// Question: https://projecteuler.net/problem=214

#include<iostream>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<fstream>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 40000000;
const ui M = 25;

vector<ui> get_totient(ui n)
{
    vector<ui> phi = vector<ui>(n+1, 1);

    for (ui i = 2; i <= n; i++)
    {
        if (phi[i] != 1)
            continue;

        for (ui j = i; j <= n; j+=i)
            phi[j] = (i-1) * phi[j];
        
        for (ui j = i*i; j <= n; j*=i)
        {
            for (ui k = j; k <= n; k += j)
                phi[k] = i * phi[k];
        }
    }
    return phi;
}

inline bool is_prime(ui n, const vector<ui>& phi)
{
    return phi[n] == n-1;
}

int main()
{   
    ui ans = 0;
    
    vector<ui> phi = get_totient(N);

    vector<ui> chain_length = vector<ui>(N+1, 0);
    chain_length[1] = 1;

    for (ui i = 2; i <= N; i++)
    {
	ui prev = phi[i];
	chain_length[i] = chain_length[prev] + 1;
    }

    for (ui i = 2; i <= N; i++)
	if (is_prime(i, phi) && chain_length[i] == M)
	    ans = ans + i;

    cout << ans << endl;

    return 0;
}
