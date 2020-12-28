#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<unordered_set>
#include<unordered_map>
#include<queue>
#include<cassert>
#include<sstream>

using namespace std;

typedef uint64_t ui;

#define endl "\n"


const ui N = 20;
const ui MOD = 1001001011ULL;

constexpr ui pow2(ui n)
{
    if (n == 0)
        return 1;
    return 2 * pow2(n-1);
};

const ui n = pow2(N); // number of numbers

ui get_next(ui m)
{
    ui bit0 = m & pow2(0);
    ui bit1 = (m & pow2(1)) >> 1ULL;
    ui bit2 = (m & pow2(2)) >> 2ULL;
    ui next_bit_n = bit0 & (bit1 ^ bit2);
    ui nn =(m >> 1ULL) | (next_bit_n << (N-1));
    return (m >> 1ULL) | (next_bit_n << (N-1));
}

void dfs(ui root, const vector<ui>& next, unordered_set<ui>& seen,
         vector<bool>& visited, vector<ui>& path)
{
    if (seen.find(root) != seen.end())
    {
        auto path_len = path.size();
        for (ui i = 0; i < path_len; i++)
            if (path[i] == root)
            {
                if (i > 0)
                    path.erase(path.begin(), path.begin() + i);
                break;
            }
        return;
    }
    if (visited[root]) // visited but not seen -> not in a cycle
    {
        path.clear();
        return;
    }
    visited[root] = true;
    seen.insert(root);
    path.push_back(root);
    dfs(next[root], next, seen, visited, path);
}

vector<vector<ui>> find_cycles(const vector<ui>& next)
{
    vector<vector<ui>> cycles;
    vector<bool> visited = vector<bool>(n, false);

    for (ui i = 0; i < n; i++)
        if (!(visited[i]))
        {
            vector<ui> path;
            unordered_set<ui> seen;
            dfs(i, next, seen, visited, path);
            if (path.size() > 0)
                cycles.push_back(path);
        }
    return cycles;
}

bool check_cycle(const vector<ui>& cycle, const vector<ui>& next)
{
    bool valid = true;
    ui len = cycle.size();
    for (ui i = 0; i < len; i++)
    {
        if (!(next[cycle[i]] == cycle[(i+1)%len]))
        {
            valid = false;
            break;
        }
    }
    return valid;
}

ui query_DP_cache(ui start, ui end, ui length, unordered_map<ui, ui>& cache)
{
    ui ans = 0;

    ui _hash = (length << 2) | (start << 1) | end;

    if (cache.find(_hash) == cache.end())
    {
        if (length == 0)
        {
            ans = 1;
        }
        else if (length == 1)
        {
            if (start == end)
                ans = 1;
            else
                ans = 0;
        }
        else
        {
            if (end == 1)
                ans = query_DP_cache(start, 0, length-1, cache);
            else
                ans = (  query_DP_cache(start, 0, length-1, cache)
                       + query_DP_cache(start, 1, length-1, cache)) % MOD;
        }
        cache[_hash] = ans;
    }

    return cache[_hash];
}

ui count_cycles_constructions(const vector<ui>& cycle, const vector<ui>& next, const vector<vector<ui>>& prev,
                              vector<ui>& n_ending_in_0, vector<ui>& n_ending_in_1, const vector<bool>& in_a_cycle,
                              vector<bool>& visited, unordered_map<ui, ui>& cache)
{
    ui ans = 0;

    vector<ui> DP_0;
    vector<ui> DP_1;

    auto cycle_length = cycle.size();

    ui pivot = -1ULL;
    for (ui i = 0; i < cycle.size(); i++)
        if (prev[cycle[i]].size() == 2)
        {
            pivot = i;
            break;
        }

    if (pivot == -1ULL) // a cycle without 3-way nodes
    {
        ans =  query_DP_cache(0, 0, cycle_length, cache)
             + query_DP_cache(0, 1, cycle_length, cache)
             + query_DP_cache(1, 0, cycle_length, cache);
        ans = ans % MOD;

        /*
        for (auto node: cycle)
        {
            assert(!visited[node]);
            visited[node] = true;
        }
        */
    }
    else
    {
        vector<ui> c; // the cycle but the first node is a 3-way node if exists
        ui i = pivot;
        for (; i < cycle.size(); i++)
            c.push_back(cycle[i]);
        for (i = 0; i != pivot; i++)
            c.push_back(cycle[i]);
        
        // assert(check_cycle(c, next));

        vector<ui> chunk_start;
        vector<ui> chunk_end;

        i = 0;
        while (i < cycle_length)
        {
            ui start = i;
            i = start + 1;
            while ((i < cycle_length) && (prev[c[i]].size() != 2))
                i++;
            chunk_start.push_back(start);
            chunk_end.push_back(i-1);
        };

        // DP[i][j][k] : number of ways of constructing the chunk i such that it starts with j and ends with k
        vector<vector<vector<ui>>> DP;

        /*
            Chunk:

       chunk_start                    chunk_end
            v                             v
            A - C - D - E - F - G - ... - Z
            |   ^^^^^^^^^^^^^^^^^^^^^^^^^^^
            B             (strand)
        */
        for (ui i = 0; i < chunk_start.size(); i++)
        {
            DP.push_back(vector<vector<ui>>(2, vector<ui>(2, 0)));
            ui chunk_length = chunk_end[i] - chunk_start[i];
            ui node_A = c[chunk_start[i]];
            ui node_Z = c[chunk_end[i]];
            ui node_B = prev[node_A][0];
            if (in_a_cycle[node_B])
                node_B = prev[node_A][1];

            /*
            //assert(!in_a_cycle[node_B]);
            //assert(visited[node_B]);

            for (ui j = chunk_start[i]; j <= chunk_end[i]; j++)
            {
                assert(!visited[c[j]]);
                visited[c[j]] = true;
            }
            */

            if (chunk_length == 0)
            {
                DP[i][0][0] = (  n_ending_in_0[node_B]          // A = 0, B = 0
                               + n_ending_in_1[node_B]) % MOD;  // A = 0, B = 1

                DP[i][0][1] = 0;

                DP[i][1][0] = 0;

                DP[i][1][1] = n_ending_in_0[node_B];            // A = 1, B = 0
            }
            else
            {
                ui n_strand_start_0_end_0 = query_DP_cache(0, 0, chunk_length, cache);
                ui n_strand_start_0_end_1 = query_DP_cache(0, 1, chunk_length, cache);
                ui n_strand_start_1_end_0 = query_DP_cache(1, 0, chunk_length, cache);
                ui n_strand_start_1_end_1 = query_DP_cache(1, 1, chunk_length, cache);

                //cout << "chunk length: " << chunk_length << endl;
                //cout << "Node B: " << node_B << "; _0:" <<  n_ending_in_0[node_B] << "; _1: " << n_ending_in_1[node_B]<< endl;

                DP[i][0][0] =   (n_ending_in_0[node_B] * n_strand_start_0_end_0) % MOD   // A = 0, B = 0, C = 0, Z = 0
                              + (n_ending_in_0[node_B] * n_strand_start_1_end_0) % MOD   // A = 0, B = 0, C = 1, Z = 0
                              + (n_ending_in_1[node_B] * n_strand_start_0_end_0) % MOD   // A = 0, B = 1, C = 0, Z = 0
                              + (n_ending_in_1[node_B] * n_strand_start_1_end_0) % MOD;  // A = 0, B = 1, C = 1, Z = 0
                DP[i][0][0] %= MOD;

                DP[i][0][1] =   (n_ending_in_0[node_B] * n_strand_start_0_end_1) % MOD   // A = 0, B = 0, C = 0, Z = 1
                              + (n_ending_in_0[node_B] * n_strand_start_1_end_1) % MOD   // A = 0, B = 0, C = 1, Z = 1
                              + (n_ending_in_1[node_B] * n_strand_start_0_end_1) % MOD   // A = 0, B = 1, C = 0, Z = 1
                              + (n_ending_in_1[node_B] * n_strand_start_1_end_1) % MOD;  // A = 0, B = 1, C = 1, Z = 1
                DP[i][0][1] %= MOD;

                DP[i][1][0] = (n_ending_in_0[node_B] * n_strand_start_0_end_0) % MOD;  // A = 1, B = 0, C = 0, Z = 0
                DP[i][1][0] %= MOD;

                DP[i][1][1] = (n_ending_in_0[node_B] * n_strand_start_0_end_1) % MOD;  // A = 1, B = 0, C = 0, Z = 1
                DP[i][1][1] %= MOD;

                if (chunk_length == 1)
                    DP[i][1][1] = 0; // A = 1 -> C = Z and C != 1
            }
        }

        if (chunk_start.size() == 1)
        {
            ans = (DP[0][0][0] + DP[0][0][1] + DP[0][1][0]) % MOD;
        }
        else
        {
            // DP2[i][j][k] : number of ways of constructing a seq of i chunks, starting with j, ending with k
            vector<vector<vector<ui>>> DP2;
            DP2.push_back(vector<vector<ui>>(2, vector<ui>(2, 0)));
            DP2[0][0][0] = DP[0][0][0];
            DP2[0][0][1] = DP[0][0][1];
            DP2[0][1][0] = DP[0][1][0];
            DP2[0][1][1] = DP[0][1][1];

            for (ui i = 1; i < chunk_start.size(); i++)
            {
                DP2.push_back(vector<vector<ui>>(2, vector<ui>(2, 0)));

                DP2[i][0][0] =   (DP2[i-1][0][0] * DP[i][0][0]) % MOD
                               + (DP2[i-1][0][0] * DP[i][1][0]) % MOD
                               + (DP2[i-1][0][1] * DP[i][0][0]) % MOD;
                DP2[i][0][0] %= MOD;

                DP2[i][0][1] =   (DP2[i-1][0][0] * DP[i][0][1]) % MOD
                               + (DP2[i-1][0][0] * DP[i][1][1]) % MOD
                               + (DP2[i-1][0][1] * DP[i][0][1]) % MOD;
                DP2[i][0][1] %= MOD;

                DP2[i][1][0] =   (DP2[i-1][1][0] * DP[i][0][0]) % MOD
                               + (DP2[i-1][1][0] * DP[i][1][0]) % MOD
                               + (DP2[i-1][1][1] * DP[i][0][0]) % MOD;
                DP2[i][1][0] %= MOD;

                DP2[i][1][1] =   (DP2[i-1][1][0] * DP[i][0][1]) % MOD
                               + (DP2[i-1][1][0] * DP[i][1][1]) % MOD
                               + (DP2[i-1][1][1] * DP[i][0][1]) % MOD;
                DP2[i][1][1] %= MOD;
            }
            ans = (DP2.back()[0][0] + DP2.back()[0][1] + DP2.back()[1][0]) % MOD;
        }

/*
        for (ui i = 0; i < chunk_start.size(); i++)
        {
            cout << "Chunk " << i << ": " << endl;
            for (ui j = chunk_start[i]; j <= chunk_end[i]; j++)
            {
                if (prev[c[j]].size() == 2)
                    cout << "[" << c[j] << "] -> ";
                else
                    cout << c[j] << " -> ";
            }
            cout << endl;
        }
*/
    }

/*
    for (auto node: cycle)
        if (prev[node].size() == 2)
            cout << "["<< node << "] -> ";
        else
            cout << node << " -> ";
    cout << endl;
    cout << "count = " << ans;
    cout << endl;
    cout << endl;
*/

    return ans;
}

int main()
{
    vector<ui> next = vector<ui>(n, 0);
    vector<vector<ui>> prev = vector<vector<ui>>(n, vector<ui>());
    vector<ui> degree_in = vector<ui>(n, 0);
    vector<ui> degree_out = vector<ui>(n, 0);
    vector<bool> in_a_cycle = vector<bool>(n, false);

    for (ui i = 0; i < n; i++)
    {
        next[i] = get_next(i);
        prev[next[i]].push_back(i);
        degree_in[next[i]] += 1;
        degree_out[i] += 1;
        //cout << i << "->" << next[i] << endl;
    }

    auto cycles = find_cycles(next);
    for (auto cycle: cycles)
        for (auto node: cycle)
            in_a_cycle[node] = true;
    
    // traverse the nodes in a topological order
    // to do this, remove all nodes that are in a cycle, and then iterate the nodes using BFS starting from the leaves
    vector<ui> n_ending_in_0 = vector<ui>(n, 0);
    vector<ui> n_ending_in_1 = vector<ui>(n, 0);
    queue<ui> traverse_queue;
    vector<bool> visited = vector<bool>(n, false);

    for (ui i = 0; i < n; i++) // all leaf nodes has in_degree of 0
        if (degree_in[i] == 0) // here, we don't need to check whether node i is in a cycle since all nodes in a cycle has in_degree of >= 1
        {
            n_ending_in_0[i] = 1;
            n_ending_in_1[i] = 1;
            traverse_queue.push(next[i]);
            visited[i] = true;
        }
    
    while (!traverse_queue.empty())
    {
        ui node = traverse_queue.front();
        traverse_queue.pop();

        // if not all children of this node were visited, ignore this node
        auto all_children_visited = true;
        for (auto p: prev[node])
            if (!visited[p])
            {
                all_children_visited = false;
                break;
            }
        if (!all_children_visited)
            continue;

        visited[node] = true;
        
        // we know that the highest in_degree is 2, so we only care about the cases where in_degree is 1 and 2
        if (degree_in[node] == 1)
        {
            auto prev_node = prev[node][0];
            n_ending_in_0[node] = (n_ending_in_0[prev_node] + n_ending_in_1[prev_node]) % MOD;
            n_ending_in_1[node] = n_ending_in_0[prev_node];
        }
        else if (degree_in[node] == 2) // 3-way merge
                                       //   A-----\
                                       //          > node
                                       //   B-----/
                                       //  A  |  B  |  C  | valid
                                       //  0  |  0  |  0  |  yes
                                       //  0  |  0  |  1  |  yes
                                       //  0  |  1  |  0  |  yes
                                       //  0  |  1  |  1  |  no (B->C is invalid, both B = C = 1)
                                       //  1  |  0  |  0  |  yes
                                       //  1  |  0  |  1  |  no (A->C is invalid, both A = C = 1)
                                       //  1  |  1  |  0  |  yes
                                       //  1  |  1  |  1  |  no (A->C and B->C are invalid, A = B = C = 1)
        {
            auto a = prev[node][0]; // prev node 1
            auto b = prev[node][1]; // prev node 2
            n_ending_in_0[node] = (  (n_ending_in_0[a] * n_ending_in_0[b]) % MOD
                                   + (n_ending_in_0[a] * n_ending_in_1[b]) % MOD
                                   + (n_ending_in_1[a] * n_ending_in_0[b]) % MOD
                                   + (n_ending_in_1[a] * n_ending_in_1[b]) % MOD) % MOD;
            n_ending_in_1[node] = (n_ending_in_0[a] * n_ending_in_0[b]) % MOD;
        }
        else
        {
            assert(false);
        }


        ui next_node = next[node];
        if (!(in_a_cycle[next_node])) // not a cycle
            traverse_queue.push(next_node);
    }

    ui ans = 1;

    unordered_map<ui, ui> cache;

    vector<ui> a;

    for (auto cycle: cycles)
    {
        ui cycle_count = count_cycles_constructions(cycle, next, prev,
                                                    n_ending_in_0, n_ending_in_1, in_a_cycle,
                                                    visited, cache);
        ans = (ans * cycle_count) % MOD;
        a.push_back(cycle_count);
    }

    cout << ans << endl;

    return 0;
}