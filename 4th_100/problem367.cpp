// Problem: https://projecteuler.net/problem=367

#include<iostream>
#include<array>
#include<vector>
#include<algorithm>
#include<bitset>
#include<unordered_map>

#include<fstream>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

constexpr ui round_to_nearest_log_2(ui n)
{
    ui exp = 0;
    ui curr = 1;
    while (curr < n)
    {
        curr = curr * 2;
        exp = exp + 1;
    }
    return exp;
}

const ui N = 11;
const ui BITS_PER_ELEMENT = round_to_nearest_log_2(N);

ui dfs(ui curr_node, ui first_node, const array<ui, N>& seq, array<bool, N>& visited)
{
    if (visited[curr_node])
        return 0;
    visited[curr_node] = true;
    return 1 + dfs(seq[curr_node], first_node, seq, visited);
}

ui find_cycle_length_started_at(ui first_node, const array<ui, N>& seq, array<bool, N>& visited)
{
    return dfs(first_node, first_node, seq, visited);
}

array<ui, N+1> find_cycle_lengths(const array<ui, N>& seq)
{
    array<ui, N+1> length_freq;
    fill(length_freq.begin(), length_freq.end(), 0);
    array<bool, N> visited;
    fill(visited.begin(), visited.end(), false);
    for (ui i = 0; i < N; i++)
        if (!visited[i])
        {
            ui cycle_length = find_cycle_length_started_at(i, seq, visited);
            length_freq[cycle_length] += 1;
        }
    return length_freq;
}

ui hash_seq(const array<ui, N>& seq)
{
    array<ui, N+1> length_freqs = find_cycle_lengths(seq);
    ui hash = 0;
    ui msb_index = 0;
    for (ui length = 0; length <= N; length++)
    {
        ui element = length_freqs[length];
        hash = hash | (element << msb_index);
        msb_index += BITS_PER_ELEMENT;
    }
    return hash;
}

int main()
{
    bitset<64> bits;
    array<ui, N> perm{0,1,2,3,4,5,6,7,8,9,10};
    ui hash = hash_seq(perm);

    unordered_map<ui, ui> freq;

    ui count = 0;

    do
    {
        ui hash = hash_seq(perm);
        if (freq.find(hash) == freq.end())
            freq[hash] = 0;
        freq[hash] += 1;
        count += 1;

    } while (next_permutation(perm.begin(), perm.end()));

    ofstream output("p367_count.txt");

    for (auto p: freq)
        output << p.first << " " << p.second << endl;

    output.close();

    return 0;
}