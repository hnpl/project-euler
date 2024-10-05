#include<iostream>
#include<cmath>
#include<stdint.h>
//#include<array>
#include<algorithm>
#include<vector>
#include<unordered_set>
#include<string>
#include<sstream>
#include<unordered_map>
#include<cassert>
#include<iomanip>

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
    if (n == 0)
        return 1;
    return 2 * pow2n(n-1);
};
const ui POW2N = pow2n(N);

typedef __uint128_t THash;
typedef uint32_t TIndex;


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
        vector<State> get_next_states()
        {
            vector<State> new_states;
            for (ui new_row_bits = 0; new_row_bits < POW2N; new_row_bits++)
            {
                //array<bool, N> new_row_mask;
                vector<bool> new_row_mask(N);
                ui bits = new_row_bits;
                for (TIndex i = 0; i < N; i++)
                {
                    new_row_mask[i] = ((bits & 1) == 1);
                    bits >>= 1;
                }

                //array<TIndex, N> new_row_color;
                vector<TIndex> new_row_colors(N);

                // Coloring passes
                // - Vertical pass
                //   + Next row has the same color as the above row
                for (ui i = 0; i < N; i++)
                {
                    if (new_row_mask[i])
                        new_row_colors[i] = this->colors[i];
                    else
                        new_row_colors[i] = 0;
                }
                // - Left-to-right pass
                for (ui i = 0; i < N-1; i++)
                {
                    if (new_row_colors[i] != 0 && new_row_mask[i+1])
                        new_row_colors[i+1] = new_row_colors[i];
                }
                // - Right-to-left pass
                for (ui i = N-1; i > 0; i--)
                {
                    if (new_row_colors[i] != 0 && new_row_colors[i-1] == 0 && new_row_mask[i-1])
                        new_row_colors[i-1] = new_row_colors[i];
                }

                // - Fill uncolored tiles
                //  + Find used colors
                //array<bool, N> used_colors;
                vector<bool> used_colors(N_COLORS);
                fill(used_colors.begin(), used_colors.end(), false);
                used_colors[0] = true;
                for (ui i = 0; i < N; i++)
                {
                    used_colors[this->colors[i]] = true;
                    used_colors[new_row_colors[i]] = true;
                }
                //  + Find empty tiles and fill it with a color
                ui next_color = 0;
                ui j = 0;
                while (j < N)
                {
                    if (new_row_colors[j] == 0 && new_row_mask[j])
                    {
                        while (used_colors[next_color])
                            next_color++;
                        used_colors[next_color] = true;
                        new_row_colors[j] = next_color;
                        j++;
                        while (j < N && new_row_colors[j] == 0 && new_row_mask[j])
                        {
                            new_row_colors[j] = new_row_colors[j-1];
                            j++;
                        }
                        j--;
                    }
                    j++;
                }
                
                // Color counting pass
                //array<ui, N> curr_to_next_map;
                vector<TIndex> curr_to_next_map(N_COLORS);
                fill(curr_to_next_map.begin(), curr_to_next_map.end(), 0);
                for (ui i = 0; i < N; i++)
                {
                    if (this->colors[i] != 0 && new_row_colors[i] != 0)
                        curr_to_next_map[this->colors[i]] = new_row_colors[i];
                }
                //array<ui, N> new_areas;
                vector<TIndex> new_areas(N_COLORS);
                fill(new_areas.begin(), new_areas.end(), 0);
                // - merging prev row
                for (ui i = 0; i < N_COLORS; i++)
                {
                    if (curr_to_next_map[i] > 0)
                        new_areas[curr_to_next_map[i]] += this->areas[i];
                }
                // - counting this row
                for (ui i = 0; i < N; i++)
                {
                    if (new_row_colors[i] > 0)
                        new_areas[new_row_colors[i]] += 1;
                }

                // Re-evaluate max_area
                TIndex new_max_area = this->max_area;
                for (ui i = 1; i < N_COLORS; i++)
                {
                    TIndex area = new_areas[i];
                    new_max_area = max(area, new_max_area);
                }

                // Recolor pass
                next_color = 1;
                //array<TIndex, N> re_color_map;
                //array<TIndex, N> recolored_new_areas;
                vector<TIndex> re_color_map(N_COLORS);
                vector<TIndex> recolored_new_areas(N_COLORS);
                fill(re_color_map.begin(), re_color_map.end(), 0);
                fill(recolored_new_areas.begin(), recolored_new_areas.end(), 0);
                ////  + Find the new color map
                for (ui i = 0; i < N; i++)
                {
                    if (new_row_colors[i] != 0)
                    {
                        if (re_color_map[new_row_colors[i]] == 0)
                        {
                            re_color_map[new_row_colors[i]] = next_color;
                            next_color++;
                        }
                    }
                }

                //  + Recoloring
                unordered_set<TIndex> new_row_colors_set;
                for (ui i = 0; i < N; i++)
                {
                    new_row_colors[i] = re_color_map[new_row_colors[i]];
                    new_row_colors_set.insert(new_row_colors[i]);
                }
                for (ui i = 0; i < N_COLORS; i++)
                {
                    if (new_row_colors_set.find(re_color_map[i]) != new_row_colors_set.end())
                        recolored_new_areas[re_color_map[i]] = new_areas[i];
                }
                State new_state = State(new_row_colors, recolored_new_areas, new_max_area);
                //State new_state = State(new_row_colors, new_areas, new_max_area);
                //cout << this->to_str() << " -> " <<new_state.to_str() << endl;
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
