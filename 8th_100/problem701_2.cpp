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

const ui N = 5;
const ui N_COLORS = N+1;
//const ui M = log((N+3)/2)/log(2);
const ui M = log(N*N)/log(2)+1;
//const ui M = N;
//const ui K = log(N*N)/log(2)+2;
//const ui K = 3;

constexpr ui pow2n(ui n)
{
    return 1 << n;
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

class State
{
    private:
        //array<TIndex, N> colors;
        //array<TIndex, N> areas;
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
        TIndex xy_to_index(const TIndex& x, const TIndex& y, const TIndex& width) const
        {
            return y*width + x;
        }
        void coloring(const TIndex& x, const TIndex& y, const vector<bool>& mask, vector<TIndex>& colors, vector<bool>& visited, const TIndex& width, const ui& picked_color) // bfs
        {
            TIndex idx = xy_to_index(x, y, width);
            queue<pair<TIndex, TIndex>> to_visit;
            to_visit.push(make_pair(x, y));
            while (!to_visit.empty())
            {
                pair<ui, ui> p = to_visit.front();
                TIndex p_x = p.first;
                TIndex p_y = p.second;
                to_visit.pop();
                idx = xy_to_index(p_x, p_y, width);
                colors[idx] = picked_color;
                visited[idx] = true;
                for (TIndex d = -1; d <= 1; d+=2)
                {
                    TIndex new_x = p_x + d;
                    if (!(new_x < 0 || new_x >= width))
                    {
                        idx = xy_to_index(new_x, p_y, width);
                        if (!visited[idx])
                            to_visit.push(make_pair(new_x, p_y));
                    }
                    TIndex new_y = p_y + d;
                    if (!(new_y < 0 || new_y > 1))
                    {
                        idx = xy_to_index(p_x, new_y, width);
                        if (!visited[idx])
                            to_visit.push(make_pair(p_x, new_y));
                    }
                }
            }

        }
        vector<State> get_next_states()
        {
            vector<State> new_states;
            for (ui new_row_bits = 0; new_row_bits < POW2N; new_row_bits++)
            {
                
                vector<bool> new_row_mask(N);
                ui bits = new_row_bits;
                for (TIndex i = 0; i < N; i++)
                {
                    new_row_mask[i] = ((bits & 1) == 1);
                    bits >>= 1;
                }
                vector<bool> mask(2*N);
                fill(mask.begin(), mask.end(), false);
                vector<TIndex> colors(2*N);
                fill(colors.begin(), colors.end(), 0);
                for (TIndex i = 0; i < N; i++)
                {
                    mask[i] = this->colors[i] != 0;
                    colors[i] = this->colors[i];
                }
                for (TIndex i = N; i < 2*N; i++)
                    mask[i] = new_row_mask[i-N];

                vector<bool> visited(2*N);
                fill(visited.begin(), visited.end(), false);
                vector<bool> used_colors(N_COLORS);
                fill(used_colors.begin(), used_colors.end(), false);
                used_colors[0] = true;
                for (TIndex i = 0; i < 2*N; i++)
                    visited[i] = !mask[i];
                //this->coloring(mask, colors, N);
                for (TIndex x = 0; x < N; x++)
                {
                    if (!visited[x])
                    {
                        TIndex picked_color = this->colors[x];
                        used_colors[picked_color] = true;
                        this->coloring(x, 0, mask, colors, visited, N, picked_color);
                    }
                }
                for (TIndex x = 0; x < N; x++)
                {
                    if (!visited[N+x])
                    {
                        TIndex picked_color = 0;
                        while (used_colors[picked_color])
                            picked_color++;
                        used_colors[picked_color] = true;
                        this->coloring(x, 1, mask, colors, visited, N, picked_color);
                    }
                }

                vector<TIndex> old_to_new_color_map(N);
                fill(old_to_new_color_map.begin(), old_to_new_color_map.end(), 0);
                for (ui i = 0; i < N; i++)
                    old_to_new_color_map[this->colors[i]] = colors[i];
                
                vector<TIndex> new_row_colors(N);
                copy(colors.begin()+N, colors.end(), new_row_colors.begin());

                vector<TIndex> new_areas(N_COLORS);
                fill(new_areas.begin(), new_areas.end(), 0);
                for (ui old_color = 1; old_color < N_COLORS; old_color++)
                    new_areas[old_to_new_color_map[old_color]] += this->areas[old_color];
                for (ui i = 0; i < N; i++)
                    new_areas[new_row_colors[i]] += 1;
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
                fill(used_colors.begin(), used_colors.end(), false);
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
                cout << curr_state.to_str() << " -> " << next_state.to_str() << " += " << curr_state_count << endl;
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

    cout << sum << endl;

    return 0;
}
