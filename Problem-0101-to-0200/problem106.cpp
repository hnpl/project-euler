// Problem: https://projecteuler.net/problem=106

/*
    . Since the second rule is satisfied, we only need to check the following:
        For all pair of disjoint sets A and B of equal size, sum(A) != sum(B).
        In other words, it must be that sum(A) > sum(B) or sum(A) < sum(B).
    . Suppose L is a sorted list, and let A and B be a pair of disjoint subset of L where A and B have the same size.
    . Let `a` be the sorted list of indicies of elements of A in L.
    . Let `b` be the sorted list of indicies of elements of B in L.
    * We have that sum(A) < sum(B) when:
        a_i < b_i for all i
    . So, we generate all possible subset A and corresponding possible subset B, and compare a_i and b_i for all i.
*/

#include<iostream>
#include<vector>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 12;

// Let L be a sorted list
// Let A and B are disjoint subsets of L
// Let `a` be the sorted list of indicies of elements in A
// Let `b` be the sorted list of indicies of elements in B
// Assume size(A) = size(B)
// Assume a[0] < b[0]
// return true if it is certain that sum(A) > sum(B) or sum(A) < sum(B)
bool has_certain_inequality(const vector<ui>& a, const vector<ui>& b)
{
    ui idx_a = 0;
    ui idx_b = 0;

    ui n = a.size();

    bool a_lt_b = true;
    // check if sum(A) < sum(B)
    for (idx_a = 1, idx_b = 1; idx_a < n; idx_a++, idx_b++) // a[0] < b[0]
        if (a[idx_a] > b[idx_b])
            return false;
    return true;
}

// generating set B
ui DFS2(ui depth, vector<ui>& path, ui prev_available_idx_idx, const vector<ui>& available_idx, const vector<ui>& setA, const ui& set_size, const ui& array_size)
{
    if (depth == set_size)
    {
        if (!has_certain_inequality(setA, path))
            return 1;
        return 0;
    }

    ui total = 0;

    ui i_lowerbound = 0;
    if (depth > 0)
        i_lowerbound = prev_available_idx_idx + 1;

    for (ui i = i_lowerbound; i < available_idx.size(); i++)
    {
        if (depth == 0 && available_idx[i] > setA[set_size - 1])
            break;
        path[depth] = available_idx[i];
        total += DFS2(depth+1, path, i, available_idx, setA, set_size, array_size);
    }

    return total;
}

// generating set A
ui DFS(ui depth, vector<ui>& path, const ui& set_size, const ui& array_size)
{
    if (depth == set_size)
    {
        vector<ui> available_idx;
        ui path_idx = 1;
        for (ui idx = path[0]+1; idx < array_size; idx++)
        {
            if (path_idx < set_size && idx == path[path_idx])
            {
                path_idx++;
                continue;
            }
            available_idx.push_back(idx);
        }
        if (available_idx.size() < set_size)
            return 0;
        vector<ui> pathB = vector<ui>(set_size);
        return DFS2(0, pathB, 0, available_idx, path, set_size, array_size);
    }
    
    ui total = 0;

    ui idx_lowerbound = 0;
    if (depth > 0)
        idx_lowerbound = path[depth-1] + 1;

    ui idx_upperbound = array_size - (set_size - depth);
    for (ui idx = idx_lowerbound; idx <= idx_upperbound; idx++)
    {
        path[depth] = idx;
        total += DFS(depth+1, path, set_size, array_size);
    }

    return total;
}

int main()
{
    ui ans = 0;

    for (ui set_size = 2; set_size <= N/2; set_size++)
    {
        vector<ui> setA = vector<ui>(set_size);
        ans = ans + DFS(0, setA, set_size, N);
    }

    cout << ans << endl;

    return 0;
}
