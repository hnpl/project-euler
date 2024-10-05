// Question: https://projecteuler.net/problem=336

#include<iostream>
#include<vector>
#include<deque>
#include<algorithm>
#include<string>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 11;
const ui M = 2011;

struct Arrangement 
{
    string str;
    Arrangement()
    {
        str = string(N, 'A');
    }
    string reverse(ui i, ui j)
    {
        string ans = str;
        for (; i < j;)
        {
            swap(ans[i], ans[j]);
            i = i + 1;
            j = j - 1;
        }
        return ans;
    }
    // fix() -> reverse the substring str[pos:] so that previously str[-1] will be in the correct place
    // reversed_fix(ch) -> reverse the substring so that the character ch will be the last character
    void reversed_fix(char ch) 
    {
        ui pos_l = str.find(ch);
        ui pos_r = str.length() - 1;
        for (; pos_l < pos_r;)
        {
            swap(str[pos_l], str[pos_r]);
            pos_l = pos_l + 1;
            pos_r = pos_r - 1;
        }
    }
    // rotate() -> reverse the substring str[pos:]
    // discover_possible_reversed_rotations() -> reverse the substring str[pos':] so that str[-1] will not be in the correct place 
    vector<string> discover_possible_reversed_rotations(ui n_fixed)
    {
        vector<string> ans;
        for (ui i = n_fixed; i < N - 1; i++)
            ans.push_back(reverse(i, N-1));
        return ans;
    }
};

bool operator<(const Arrangement& lhs, const Arrangement& rhs)
{
    return lhs.str < rhs.str;
}

ui factorial(ui n)
{
    ui ans = 1;
    for (ui i = 2; i <= N; i++)
        ans = ans * i;
    return ans;
}

int main()
{

    deque<Arrangement> q;
    deque<ui> depth;
    Arrangement final_state;
    for (ui i = 0; i < N; i++)
        final_state.str[i] = 'A' + i;
    final_state.str = final_state.discover_possible_reversed_rotations(N-2)[0];
    final_state.reversed_fix(final_state.str[N-3]);
    final_state.str = final_state.discover_possible_reversed_rotations(N-2)[0];
    q.push_back(final_state);
    depth.push_back(3);

    while (depth.front() != 2*N - 3)
    {
        auto curr_state = q.front();
        q.pop_front();
        auto curr_depth = depth.front();
        depth.pop_front();
        auto next_depth = curr_depth + 1;
        
        if (next_depth % 2 == 0) // reversed_fix
        {
            Arrangement next_state;
            next_state.str = curr_state.str;
            next_state.reversed_fix('A' + N - 2 - next_depth / 2);
            q.push_back(next_state);
            depth.push_back(next_depth);
        }
        else // reversed_rotation
        {
            auto next_state_str = curr_state.discover_possible_reversed_rotations(N - 1 - next_depth / 2);
            for (auto state: next_state_str)
            {
                Arrangement next_state;
                next_state.str = state;
                q.push_back(next_state);
                depth.push_back(next_depth);
            }    
        }
    }

    sort(q.begin(), q.end());
    auto ans_arrangement = q[M-1];
    cout << ans_arrangement.str << endl;

    return 0;
}