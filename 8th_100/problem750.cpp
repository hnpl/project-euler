// Question: https://projecteuler.net/problem=750

/*
 * . In the optimal dragging sequence of card [1..N], we do not move the card N.
 * . So, the optimal number of moves to put [1..N] in order is,
 *      DP[1..N] = DP[1..k] + DP[k+1..N] + cost_of_moving(from the position of card k to the position of card N)  
 */

#include<iostream>
#include<vector>
#include<cassert>

using namespace std;

typedef uint64_t ui;

const ui N = 976;
const ui M = 3;
const ui INF = 1ULL << 63;

class SequenceGenerator
{
    private:
        ui next;
        ui n;
        ui seed;
    public:
        SequenceGenerator(const ui& n, const ui& seed)
        { 
            this->seed = seed;
            this->next = this->seed;
            this->n = n;
        }
        ui get_next()
        {
            ui ans = this->next;
            this->next = (this->next * this->seed) % (n+1);
            return ans;
        }
        void reset()
        {
            this->next = this->seed;
        }
};

ui abs_sub(const ui& a, const ui& b)
{
    if (a > b)
        return a - b;
    return b - a;
}

ui G(const ui& n)
{
    ui ans = 0;

    SequenceGenerator seq(n, M);
    vector<ui> card_pos(n+1, 0);
    for (auto i = 0; i < n; i++)
        card_pos[seq.get_next()] = i+1;
    
    vector<vector<ui>> DP = vector<vector<ui>>(n+1, vector<ui>(n+1, INF));
    for (auto i = 0; i <= n; i++)
        DP[i][i] = 0;

    for (auto length = 2; length <= n; length++)
    {
        for (auto start = 1; start + length - 1 <= n; start++)
        {
            ui end = start + length - 1;
            for (auto mid = start; mid < end; mid++)
            {
                DP[start][end] = min(DP[start][end],
                                     DP[start][mid] + DP[mid+1][end] + abs_sub(card_pos[mid], card_pos[end])
                                    );
            }
        }
    }

    ans = DP[1][n];

    return ans;
}

int main()
{
    cout << F(N) << endl;
    return 0;
}
