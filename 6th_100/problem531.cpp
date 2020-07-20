// Question: https://projecteuler.net/problem=531
// g++ problem531.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -lpari -O3

#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui L = 1000000;
const ui R = 1005000;


inline ui f(ui n, ui m, const vector<ui>& phi)
{
    ui ans = 0;

    pari_CATCH(CATCH_ALL)
    {
	GEN error = pari_err_last();
	if (err_get_num(error) != 29) // error 29: inconsistent congruences
	    pari_err(0, error);
	    ans = 0;
    }
    pari_TRY
    {
	    GEN crt = chinese(gmodulss(phi[n], n), gmodulss(phi[m], m));
	    ans = gtolong(gel(crt, 2));
    }
    pari_ENDCATCH;

    return ans;
}

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

int main()
{
    pari_init(5000000000,2); // 5 GB

    ui ans = 0;

    vector<ui> phi = get_totient(R);

    for (ui n = L; n < R; n++)
	for (ui m = n + 1; m < R; m++)
	    ans = ans + f(n, m, phi);

    pari_close();

    cout << ans << endl;

    return 0;
}
