// Question: https://projecteuler.net/problem=303

// This is a milder version of problem 714.

// We will iterate through all posibilities of number constructed from 0, 1, and 2 using BFS.
//     - For each number in the queue, we will concatenate digits 0, 1, and 2 and put them to the queue.
//       So, we know that the numbers in the queue are in order.
//     - Note that, we are finding the number having modulo n of 0.
//     - If two numbers have the same modulo n, the first one appears in the queue is smaller.
//       Other than that, if we add a digit in {0, 1, 2} to the right of those two numbers, we will produce
//       two new numbers of the same modulo n, and of the two new numbers, the smaller one is produced by
//       adding the digit from the smaller number of the two original number.
//       i.e. Suppose a and b have the same modulo n, and a < b.
//            Suppose a' = 10*a + d, b' = 10*b + d, where d is a digit.
//            Then a' < b', and (a' mod n) = (b' mod n).
//       So, we only need to visit a number corresponding to one modulus n once.

#include<iostream>
#include<unordered_set>
#include<queue>

using namespace std;

#define endl "\n"

typedef uint64_t ui;

const ui N = 10000;

inline ui modulo(const ui& lhs, const ui& rhs)
{
    ui ans = lhs;
    while (ans >= rhs)
        ans = ans - rhs;
    return ans;
}

inline ui BFS(ui n)
{
    if (n == 1 || n == 2)
        return n;

    queue<ui> mod_q;
    queue<ui> num_q;

    unordered_set<ui> visited;

    mod_q.push(modulo(1, n));
    num_q.push(1);
    visited.insert(1);

    mod_q.push(modulo(2,n));
    num_q.push(2);
    visited.insert(2);

    while (!mod_q.empty())
    {
        auto mod = mod_q.front(); mod_q.pop();
        auto num = num_q.front(); num_q.pop();

        for (ui digit = 0; digit <= 2; digit++)
        {
            auto mod_k = modulo(mod * 10 + digit, n);
            auto num_k = num * 10 + digit;
            if (mod_k == 0)
                return num_k;
            if (visited.find(mod_k) == visited.end())
            {
                mod_q.push(mod_k);
                num_q.push(num_k);
                visited.insert(mod_k);
            }
        }
    }

    return -1;
}

inline ui f(ui n)
{
    return BFS(n) / n;
}

int main()
{
    ui ans = 0;
    for (ui n = 1; n <= N; n++)
        ans = ans + f(n);
    cout << ans << endl;
    return 0;
}
