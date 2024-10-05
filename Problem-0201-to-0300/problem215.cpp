// Problem: https://projecteuler.net/problem=215

/*
    - Let layer be a way of placing bricks of lengths {2,3} on an entire row.

    - Let f(layer) : bits be a function encoding a layer
        - bits[i] is set if one of the bricks of the layer has one of its right end in position i.
        - since the-most-rightward-bricks of all rows always have their right end aligned, we don't encode this bit (bit[N_COLS]) to simplify the logic.

    - We can check if 2 layers a, b (with layer a being placed on top of layer b or vice versa) has a crack by checking bitwise-AND(a, b).
        - bitwise-AND(a,b) != 0 if it has a crack.
        - bitwise-AND(a,b) = 0 otherwise.

    - Let DP[i][j] denote the number of ways of placing the bricks on row i with the row i has layer of K where f(K) = j.
        - Then, DP[i][j] = sum_{all q where bitwise-AND(q,j) == 0} DP[i-1][q]

*/

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<bitset>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N_ROWS = 10;
const ui N_COLS = 32;

const vector<ui> BRICK_LENGTHS = {2, 3};

ui encode_layer(vector<ui>& path, ui n) // n: actual size of paths
{
    ui ans = 0;

    ui curr_length = -1;

    for (ui i = 0; i < n - 1; i++) // since the last bricks are always aligned at N_COLS across all rows
                                   // i.e. bits[N_COLS] = 1 for all possible layer
                                   // it's unnecessary to encode it
    {
        curr_length = curr_length + path[i];
        ans = ans | (1 << curr_length);
    }

    return ans;
}

void DFS(ui depth, vector<ui>& path, const ui curr_length, const ui& target_length, vector<ui>& layers)
{
    if (curr_length == target_length)
    {
        layers.push_back(encode_layer(path, depth));
        return;
    }

    for (auto brick_length: BRICK_LENGTHS)
    {
        if (curr_length + brick_length > target_length)
            break; // assume that BRICK_LENGTHS is sorted
        path[depth] = brick_length;
        DFS(depth+1, path, curr_length + brick_length, target_length, layers);
    }
}

vector<ui> generate_layers()
{
    vector<ui> possible_layers;

    vector<ui> path = vector<ui>(N_COLS / (*min_element(BRICK_LENGTHS.begin(), BRICK_LENGTHS.end())) + 1);

    DFS(0, path, 0, N_COLS, possible_layers);

    return possible_layers;
}

int main()
{
    ui ans = 0;

    vector<ui> possible_layers = generate_layers(); // layer: placing bricks on an entire row

    ui n = possible_layers.size();

    vector<vector<ui>> DP = vector<vector<ui>>(2, vector<ui>(n, 0));
    ui curr_row = 0;
    ui prev_row = 1 - curr_row;
    fill(DP[curr_row].begin(), DP[curr_row].end(), 1); // placing the first row

    for (ui i = 1; i < N_ROWS; i++)
    {
        curr_row = i % 2;
        prev_row = 1 - curr_row;

        fill(DP[curr_row].begin(), DP[curr_row].end(), 0);

        for (ui curr_layer_idx = 0; curr_layer_idx < n; curr_layer_idx++)
        {
            ui curr_layer = possible_layers[curr_layer_idx];
            for (ui prev_layer_idx = 0; prev_layer_idx < n; prev_layer_idx++)
            {
                ui prev_layer = possible_layers[prev_layer_idx];
                if ((curr_layer & prev_layer) == 0)
                    DP[curr_row][curr_layer_idx] += DP[prev_row][prev_layer_idx];
            }
        }
    }

    ans = accumulate(DP[curr_row].begin(), DP[curr_row].end(), 0ULL);

    cout << ans << endl;

    return 0;
}


