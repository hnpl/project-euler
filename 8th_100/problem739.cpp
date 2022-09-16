// Question: https://projecteuler.net/problem=739

// g++ problem739.cpp -I$HOME/pari/include/ -L$HOME/pari/lib -lpari -O3

/*
 * . Consider f(8) with starting sequence of [1,1,1,1,1,1,1,1].
 * . Let v = (v1, v2, v3, v4, v5, v6, v7, v8) where v_j is the contribution of the j^th element to the final sum (i.e., the number of times v_j appears in the final summation).
 * . So, we can represent the starting sequence as,
 *   [1,1,1,1,1,1,1,1] = [(1,0,0,0,0,0,0,0),
 *                        (0,1,0,0,0,0,0,0),
 *                        (0,0,1,0,0,0,0,0),
 *                        (0,0,0,1,0,0,0,0),
 *                        (0,0,0,0,1,0,0,0),
 *                        (0,0,0,0,0,1,0,0),
 *                        (0,0,0,0,0,0,1,0),
 *                        (0,0,0,0,0,0,0,1)]
 * . So, at each step, we use element wise vector adding for adding two elements.
 * . A quick python script should generate the contribution of each initial element to the final sum,
 *   sage: def add(l1, l2):
 *   ....:     l = []
 *   ....:     for i in range(len(l1)):
 *   ....:         l.append(l1[i] + l2[i])
 *   ....:     return l
 *   ....:
 *   sage: initial = [[0]*8 for i in range(8)]
 *   sage: for i in range(8):                               
 *   ....:     initial[i][i]=1
 *   ....:
 *   sage: def f(initial, n): 
 *   ....:     curr = initial[:]
 *   ....:     prev = []
 *   ....:     for step in range(n-1):
 *   ....:         prev, curr = curr, []
 *   ....:         running_sum = prev[1][:]
 *   ....:         curr.append(running_sum)
 *   ....:         for i in range(n-2-step):
 *   ....:             running_sum = add(running_sum, prev[i+2])
 *   ....:             curr.append(running_sum)
 *   ....:         print(curr)
 *   ....:         print()
 *   ....:     return curr
 *   ....:
 *   sage: x = f(initial, 8)
 *   [[0, 1, 0, 0, 0, 0, 0, 0], [0, 1, 1, 0, 0, 0, 0, 0], [0, 1, 1, 1, 0, 0, 0, 0], [0, 1, 1, 1, 1, 0, 0, 0], [0, 1, 1, 1, 1, 1, 0, 0], [0, 1, 1, 1, 1, 1, 1, 0], [0, 1, 1, 1, 1, 1, 1, 1]]
 *   [[0, 1, 1, 0, 0, 0, 0, 0], [0, 2, 2, 1, 0, 0, 0, 0], [0, 3, 3, 2, 1, 0, 0, 0], [0, 4, 4, 3, 2, 1, 0, 0], [0, 5, 5, 4, 3, 2, 1, 0], [0, 6, 6, 5, 4, 3, 2, 1]]
 *   [[0, 2, 2, 1, 0, 0, 0, 0], [0, 5, 5, 3, 1, 0, 0, 0], [0, 9, 9, 6, 3, 1, 0, 0], [0, 14, 14, 10, 6, 3, 1, 0], [0, 20, 20, 15, 10, 6, 3, 1]]
 *   [[0, 5, 5, 3, 1, 0, 0, 0], [0, 14, 14, 9, 4, 1, 0, 0], [0, 28, 28, 19, 10, 4, 1, 0], [0, 48, 48, 34, 20, 10, 4, 1]]
 *   [[0, 14, 14, 9, 4, 1, 0, 0], [0, 42, 42, 28, 14, 5, 1, 0], [0, 90, 90, 62, 34, 15, 5, 1]]
 *   [[0, 42, 42, 28, 14, 5, 1, 0], [0, 132, 132, 90, 48, 20, 6, 1]]
 *   [[0, 132, 132, 90, 48, 20, 6, 1]]
 *
 *   . As we can see, [[0, 132, 132, 90, 48, 20, 6, 1]] is the contribution of each element to the sum.
 *   . Note that, 1, 6, 20, 48, 90, 132, 132 is the 6^th row of the Catalan's Triangle https://en.wikipedia.org/wiki/Catalan%27s_triangle.
 *   . Let C(n, k) denote the Catalan number at row n and column k.
 *     From the Wikipedia page, we have, 
 *          C(n, k) = (n-k+1)/(n+1) * binomial(n+k,k)
 *     Thus,
 *          C(n, k+1) = C(n,k) * (n-k)/(n-k+1)*(n+k+1)/(k+1)
 *   . So, basically, the contribution of each element to the sum is the reverse of the (n-2)^th row of the Catalan's Triangle.
 *   . We can generate the Catalan's Triangle's (N-2)^th row in order, and generate the Lucas sequence in the reversed order, then sum the element-wise product of them.
 */


#include<pari/pari.h>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>

using namespace std;

typedef int64_t ui;

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = pow(10, 8);
const ui MOD = pow(10, 9) + 7;

ui inv_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

ui Catalan_table_row_generator(const ui& n, const ui& k, const ui& prev)
{
    ui ans = prev;
    ans *= n-k+1;
    ans %= MOD;
    ans *= inv_mod(n-k+2, MOD);
    ans %= MOD;
    ans *= n + k;
    ans %= MOD;
    ans *= inv_mod(k, MOD);
    ans %= MOD;
    return ans;
}

vector<ui> Lucas_sequence_generator(const ui& n)
{
    vector<ui> seq = vector<ui>(n, 0);
    seq[0] = 1;
    seq[1] = 3;
    ui a = 1;
    ui b = 3;
    for (ui i = 2; i < n; i++)
    {
        ui old_b = b;
        b = a + b;
        b %= MOD;
        a = old_b;
        seq[i] = b;
    }
    return seq;
}

int main()
{
    ui ans = 0;
    ui curr = 1;
    auto Lucas_sequence = Lucas_sequence_generator(N);
    auto it = Lucas_sequence.rbegin();
    ans += (*it) * curr;
    ans %= MOD;
    it++;
    for (ui k = 1; k <= N-2; k++)
    {
        curr = Catalan_table_row_generator(N-2, k, curr);
        ans += (*it) * curr;
        ans %= MOD;
        it++;
    }
    cout << ans << endl;
    return 0;
}
