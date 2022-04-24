// Question: https://projecteuler.net/problem=701

/*
 * Stategy:
 *   - State = the color of the last row + the area of each color + the maximum area.
 *      + color 0: the block should not be recolored.
 *      + color != 0: same color means the blocks are connected (can be connected due to many rows before this row.)
 *   - Keep adding more row to each state.
 *   - Use BFS to recolor the last two rows, then recalculate the area and maximum area.
 *   - The previous approaches avoided using grpahs but failed to produce correct result,
 *     - Case 1: problem701_1.cpp failed in the case
 *       old_row = [1, 0, 2, 2, 2]
 *       new_row = [x, x, x, 0, x]
 *       result  = old [1, 0, 2, 2, 2]
 *                 new [1, 1, 1, 0, 2]
 *       result should be: old [1, 0, 1, 1, 1]
 *                         new [1, 1, 1, 0, 1]
 *     - Case 2: problem701_2.cpp failed in the case
 *       old_row = [1, 0, 2, 0, 2]
 *       new_row = [x, x, x, 0, 0]
 *       result  = old [1, 0, 1, 0, 2]
 *                 new [1, 1, 1, 0, 0]
 *       result should be: old [1, 0, 1, 0, 1]
 *                         new [1, 1, 1, 0, 0]
 *    Interestingly, problem701_1.cpp didn't fail in case 2, and problem701_2.cpp didn't fail in case 1.
 *    So, analyzing two incorrect graph coloring implementations results in the correct implementation.
*/

#include<iostream>
#include<cmath>
#include<stdint.h>
//#include<array>
#include<algorithm>
#include<vector>
#include<unordered_set>
#include<set>
#include<string>
#include<sstream>
#include<unordered_map>
#include<cassert>
#include<iomanip>
#include<queue>
#include<utility>

using namespace std;

typedef uint64_t ui;

#define endl "\n"

const ui N = 7;
const ui N_COLORS = N+1;
const ui M = log(N*N)/log(2)+1;

constexpr ui pow2n(ui n)
{
    return 1ULL << n;
};
const ui POW2N = pow2n(N);

typedef __uint128_t THash;
typedef int16_t TIndex;


string to_string(__uint128_t n)
{
    stringstream stream;
    while (n > 0)
    {
        stream << char('0' + n % 10);
        n = n / 10;
    }
    string str = stream.str();
    reverse(str.begin(), str.end());
    if (str == "")
        return "0";
    return str;
}

ostream& operator<<(ostream& os, const __uint128_t n)
{
    os << to_string(n);
    return os;
}

class ColoredTwoRowGraph // this class is made specically for this problem, should not be reused
{
    private:
        TIndex n_nodes;
        vector<vector<TIndex>> neighbors;
        vector<TIndex> colors;
        void coloring(const TIndex& starting_node, const TIndex& picked_color, vector<bool>& visited)
        {
            queue<TIndex> to_visit;
            to_visit.push(starting_node);
            while (!to_visit.empty())
            {
                TIndex u = to_visit.front();
                to_visit.pop();
                visited[u] = true;
                colors[u] = picked_color;

                for (auto neighbor: neighbors[u])
                {
                    if (!visited[neighbor])
                        to_visit.push(neighbor);
                }
            }
        }
    public:
        ColoredTwoRowGraph() {}
        // colors -> colors of 2 rows
        // Color 0 -> should not be recolored
        ColoredTwoRowGraph(const TIndex& n_nodes, const vector<TIndex>& colors)
        {
            this->n_nodes = n_nodes;
            this->neighbors = vector<vector<TIndex>>(n_nodes);
            for (ui i = 0; i < n_nodes; i++)
                this->neighbors[i] = vector<TIndex>();
            this->colors = vector<TIndex>(n_nodes);
            copy(colors.begin(), colors.end(), this->colors.begin());
        }
        void add_edge(const TIndex& u, const TIndex& v)
        {
            this->neighbors[u].push_back(v);
            this->neighbors[v].push_back(u);
        }
        void recolor() // using BFS to find the optimial coloring using the least amount of colors
        {
            TIndex next_color = 1;
            vector<bool> visited(n_nodes, false);
            for (TIndex u = 0; u < n_nodes; u++)
                visited[u] = (colors[u] == 0); // avoid visiting nodes that are not colorable
            for (TIndex u = 0; u < n_nodes; u++)
            {
                if (!visited[u])
                {
                    TIndex picked_color = next_color;
                    next_color += 1;
                    this->coloring(u, picked_color, visited);
                }
            }
        }
        vector<TIndex> get_colors()
        {
            return this->colors;
        }
};

class State
{
    private:
        vector<TIndex> colors;
        vector<TIndex> areas;
        THash hash;
        TIndex max_area;
        THash calculate_hash()
        {
            THash hash = 0;
            THash msb = 0;
            for (TIndex i = 0; i < N; i++)
            {
                hash |= (((THash)colors[i]) << msb);
                msb += M;
            }
            for (TIndex i = 0; i < N_COLORS; i++)
            {
                hash |= (((THash)areas[i]) << msb);
                msb += M;
            }
            hash |= (((THash)max_area) << msb);
            return hash;
        }
    public:
        vector<TIndex> get_colors()
        {
            return this->colors;
        }
        vector<TIndex> get_areas()
        {
            return this->areas;
        }
        string to_str()
        {
            stringstream s;
            s << "colors[";
            for (auto color: this->colors)
                s << color << ", ";
            s << "]; areas [";
            for (auto area: this->areas)
                s << area << ", ";
            s << "] max_area=";
            s << this->max_area;
            return s.str();
        }
        State()
        {
            fill(this->colors.begin(), this->colors.end(), 0);
            fill(this->areas.begin(), this->areas.end(), 0);
            this->max_area = 0;
            this->hash = 0;
        }
        //State(const array<TIndex, N>& colors, const array<TIndex, N>& areas, const TIndex& max_area)
        State(const vector<TIndex>& colors, const vector<TIndex>& areas, const TIndex& max_area)
        {
            this->colors = vector<TIndex>(N);
            this->areas = vector<TIndex>(N_COLORS);
            copy(colors.begin(), colors.end(), this->colors.begin());
            copy(areas.begin(), areas.end(), this->areas.begin());
            this->max_area = max_area;
            this->hash = this->calculate_hash();
        }
        State(THash hash)
        {
            this->colors = vector<TIndex>(N);
            this->areas = vector<TIndex>(N_COLORS);
            this->hash = hash;
            for (TIndex i = 0; i < N; i++)
            {
                this->colors[i] = hash & (pow2n(M) - 1);
                hash >>= M;
            }

            for (TIndex i = 0; i < N_COLORS; i++)
            {
                this->areas[i] = hash & (pow2n(M) - 1);
                hash >>= M;
            }

            this->max_area = hash;
        }
        THash get_hash()
        {
            return this->hash;
        }
        TIndex get_max_area()
        {
            return this->max_area;
        }
        vector<State> get_next_states()
        {
            vector<State> new_states;
            for (ui new_row_bits = 0; new_row_bits < POW2N; new_row_bits++)
            {
                vector<bool> new_row_mask(N);
                ui bits = new_row_bits;
                for (TIndex x = 0; x < N; x++)
                {
                    new_row_mask[x] = ((bits & 1) == 1);
                    bits >>= 1;
                }

                // initially, the colors inputted to the graph will act like a mask (whether a node should be recolored)
                // the edges will specify which nodes should have the same color
                // row 1 -> current row
                // row 2 -> next row
                vector<TIndex> two_row_colors(2*N, 0);
                for (ui x = 0; x < N; x++)
                {
                    two_row_colors[x] = (this->colors[x] != 0) ? 1 : 0;
                    two_row_colors[N+x] = new_row_mask[x] ? 1 : 0;
                }

                ColoredTwoRowGraph graph = ColoredTwoRowGraph(2*N, two_row_colors);

                // Adding edges for the current row
                // For the current row, if the two nodes sharing the same color then they are connected
                for (TIndex u = 0; u < N; u++)
                {
                    if (this->colors[u] == 0)
                        continue;
                    for (TIndex v = u+1; v < N; v++)
                        if (this->colors[v] == this->colors[u])
                            graph.add_edge(u, v);
                }
                // Adding edges for the new row
                // For the new row, if two adjacent nodes are 1 then they will be conencted
                // We don't need to check non-adjacent nodes like in the current row because we don't know if two non-adjacent nodes in the new row should be connected
                // If they are connected, they will have the same color after the recoloring phase
                ui x = 0;
                while (x < N)
                {
                    if (new_row_mask[x])
                    {
                        while (x+1 < N)
                        {
                            graph.add_edge(N+x, N+x+1);
                            x++;
                        }
                    }
                    x++;
                }

                // adding edges across two rows
                for (ui x = 0; x < N; x++)
                {
                    if (two_row_colors[x] != 0 && two_row_colors[N+x] != 0)
                        graph.add_edge(x, N+x);
                }

                graph.recolor();

                vector<TIndex> new_coloring = graph.get_colors();
                vector<TIndex> curr_row_colors(N);
                copy(new_coloring.begin(), new_coloring.begin()+N, curr_row_colors.begin());
                vector<TIndex> new_row_colors(N);
                copy(new_coloring.begin()+N, new_coloring.end(), new_row_colors.begin());

                vector<TIndex> old_to_new_color_map(N);
                fill(old_to_new_color_map.begin(), old_to_new_color_map.end(), 0);
                for (ui x = 0; x < N; x++)
                    old_to_new_color_map[this->colors[x]] = curr_row_colors[x];

                vector<TIndex> new_areas(N_COLORS, 0);
                for (ui old_color = 1; old_color < N_COLORS; old_color++)
                    new_areas[old_to_new_color_map[old_color]] += this->areas[old_color];
                for (ui x = 0; x < N; x++)
                    new_areas[new_row_colors[x]] += 1;
                new_areas[0] = 0;
                TIndex new_max_area = this->max_area;
                for (ui i = 1; i < N_COLORS; i++)
                    new_max_area = max(new_max_area, new_areas[i]);
                set<TIndex> new_row_color_set;
                for (ui i = 0; i < N; i++)
                    new_row_color_set.insert(new_row_colors[i]);

                for (ui color = 1; color < N_COLORS; color++)
                    if (new_row_color_set.find(color) == new_row_color_set.end())
                        new_areas[color] = 0;

                vector<TIndex> recolored_new_row_colors(N, 0);
                vector<TIndex> recolored_new_areas(N_COLORS, 0);
                vector<TIndex> recoloring_map(N_COLORS, 0);
                vector<bool> used_colors(N_COLORS, false);
                used_colors[0] = true;
                for (ui i = 0; i < N; i++)
                {
                    if (new_row_mask[i])
                    {
                        TIndex old_color = new_row_colors[i];
                        if (recoloring_map[old_color] != 0)
                            continue;
                        TIndex new_color = 0;
                        while (used_colors[new_color])
                            new_color++;
                        used_colors[new_color] = true;
                        recoloring_map[old_color] = new_color;
                    }
                }
                for (ui x = 0; x < N; x++)
                    recolored_new_row_colors[x] = recoloring_map[new_row_colors[x]];
                for (ui old_color = 1; old_color < N_COLORS; old_color++)
                    recolored_new_areas[recoloring_map[old_color]] = new_areas[old_color];

                State new_state = State(recolored_new_row_colors, recolored_new_areas, new_max_area);
                //State new_state = State(new_row_colors, new_areas, new_max_area);
                new_states.push_back(new_state);

            }
            return new_states;
        }
};

int main()
{
    __uint128_t ans = 0;

    State initial_state = State();

    unordered_map<THash, ui> map1;
    unordered_map<THash, ui> map2;

    unordered_map<THash, ui>& curr_states = map1;
    unordered_map<THash, ui>& next_states = map2;

    next_states[initial_state.get_hash()] = 1;

    cout << (2*N+2)*M << endl;

    for (ui i = 0; i < N; i++)
    {
        swap(curr_states, next_states);
        next_states.clear();

        for (auto p: curr_states)
        {
            THash curr_state_hash = p.first;
            State curr_state = State(curr_state_hash);
            ui curr_state_count = p.second;
            for (auto next_state: curr_state.get_next_states())
            {
                THash next_state_hash = next_state.get_hash();
                if (next_states.find(next_state_hash) == next_states.end())
                   next_states[next_state_hash] = 0;
                next_states[next_state_hash] += curr_state_count;
                //State tmp = State(next_state.get_colors(), next_state.get_areas(), next_state.get_max_area());
                //assert(tmp.get_hash() == next_state_hash);
                //cout << curr_state.to_str() << " -> " << next_state.to_str() << " += " << curr_state_count << endl;
            }
        }
    }

    ui sum = 0;
    for (auto p: next_states)
    {
        THash next_state_hash = p.first;
        State next_state = State(next_state_hash);
        ui next_state_count = p.second;

        ans += p.second * next_state.get_max_area();
        cout << p.second << " " <<  next_state.to_str() <<endl;
        sum += next_state_count;
    }

    cout << ans << endl;

    double d_ans = 1.0 * ans / pow2n(N*N);

    cout << fixed << setprecision(10) << d_ans << endl;

    //cout << sum << endl;
    assert(sum == pow2n(N*N));

    return 0;
}
