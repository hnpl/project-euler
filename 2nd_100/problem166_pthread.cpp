// Problem: https://projecteuler.net/problem=166

#include<iostream>
#include<vector>
#include<thread>

using namespace std;

typedef int64_t i64;

#define endl "\n"

inline bool is_digit(int n)
{
    return (n >= 0) && (n <= 9);
}

int check_board(const vector<int>& board)
{
    /*
        a1 b2 c3  x1
        d4 e5 f6  x2
        g7 h8 i9  x3
        x4 x5 x6  x7

        For each (a1, b2, c3, d4, e5, f6, g7, h8, i9), a solution (x1, x2, x3, x4, x5, x6, x7) (if exists) is unique.
        Why? 
            Suppose there are two different solutions, (x1, ..., x7) and (y1, ..., y7).
            Let p = a1 + b2 + c3 + x1 and q = a1 + b2 + c3 + y1 and p != q.
            So, x1 - y1 = p - q.

            We have p = x1 + f6 + h8 + x4.
            Also, q = y1 + f6 + h8 + y4.
            So, p - q = x1 - y1 + x4 - y4.
            Therefore, 0 = x4 - y4, or x4 = y4, which means p = a1 + d4 + g7 + x4 = a1 + d4 + g7 + y4 = q, or p = q (contradiction).

            Therefore, for each (a1, b2, c3, d4, e5, f6, g7, h8, i9), a solution (x1, x2, x3, x4, x5, x6, x7) (if exists) is unique.
    */
    int d = board[1] + board[2] + board[3];
    for (int x1 = 0; x1 <= 9; x1++)
    {
        int sum = x1 + d;
        int x2 = sum - board[4] - board[5] - board[6];
        int x3 = sum - board[7] - board[8] - board[9];
        int x4 = sum - board[1] - board[4] - board[7];
        int x5 = sum - board[2] - board[5] - board[8];
        int x6 = sum - board[3] - board[6] - board[9];
        int x7 = sum - board[1] - board[5] - board[9];
        if (((x1+x2+x3) == (x4+x5+x6)) && ((x1 + x2 + x3 + x7)==sum) && ((x1 + board[6] + board[8] + x4) == sum) 
                && is_digit(x2) && is_digit(x3) && is_digit(x4) && is_digit(x5) && is_digit(x6) && is_digit(x7))
            return sum;
    }
    return -1;
}

i64 DFS(i64 depth, vector<int>& board)
{
    if (depth == 9)
    {
        i64 total = 0;
        for (int i = 0; i <= 9; i++)
        {
            board[depth] = i;
            int sum = check_board(board);
            if (sum >= 0)
                total = total + 1;
        }
        return total;
    }

    i64 total = 0;
    for (int i = 0; i <= 9; i++)
    {
        board[depth] = i;
        total = total + DFS(depth+1, board);
    }

    return total;
}

struct ThreadResult
{
    int ans;
};

void worker(i64 first_number, ThreadResult& output)
{
    vector<int> board = vector<int>(10);
    board[1] = first_number;
    output.ans = DFS(2, board);
}

int main()
{
    i64 ans = 0;

    i64 n_threads = 10;

    vector<thread> threads;
    vector<ThreadResult> thread_results = vector<ThreadResult>(n_threads);

    for (int i = 0; i < n_threads; i++)
    {
        thread new_thread(worker, i, ref(thread_results[i]));
        threads.push_back(move(new_thread));
    }
    

    for (int i = 0; i < n_threads; i++)
        threads[i].join();

    for (auto& result: thread_results)
        ans = ans + result.ans;

    cout << ans << endl;

    return 0;
}
