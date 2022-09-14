// Question: https://projecteuler.net/problem=534

// Either dfs all possible queen configurations, or keeping track of the previous N-1-w rows and using dynamic programming.

#include<iostream>
#include<unordered_map>
#include<vector>
#include<array>
#include<algorithm>
#include<sstream>
#include<string>

using namespace std;

typedef int64_t ui;
typedef uint64_t THash;
typedef uint8_t TElement;
#define endl "\n"

const ui N = 14;
const ui ELEMENT_WIDTH_BITS = 4; // element \in [0..7] are reserved for radius, elements that are >= 8 are invalid
const TElement INVALID_POS = 15;

constexpr THash get_mask(const ui& n)
{
    return (((THash)1) << n) - 1;
};

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

bool check(const ui& queen1_row, const ui& queen1_col, const ui& queen2_row, const ui& queen2_col)
{
    if (queen1_col == queen2_col)
        return false;

    ui dy = queen2_row - queen1_row; // dy < n - w - 1
    ui dx = queen2_col - queen1_col;
    
    if ((dx == dy) || (dx == -dy))
        return false;

    return true;
}

ui max(const ui& a, const ui& b)
{
    if (a > b)
        return a;
    return b;
}

ui dfs(const ui& depth, array<ui, N>& pos, const ui& max_depth, const ui& radius)
{
    if (depth == max_depth)
        return 1;
    
    ui ans = 0;

    for (ui col = 0; col < N; col++)
    {
        bool is_possible = true;
        for (ui prev_queen_row = max(0ULL, depth - radius); prev_queen_row < depth; prev_queen_row++)
        {
            ui prev_queen_col = pos[prev_queen_row];
            if (!check(depth, col, prev_queen_row, prev_queen_col))
            {
                is_possible = false;
                break;
            }
        }

        if (!is_possible)
            continue;

        pos[depth] = col;
        ans += dfs(depth+1, pos, max_depth, radius);
    }

    return ans;
}

class State
{
    private:
        vector<TElement> pos;
        THash hash;
        static vector<unordered_map<THash, unordered_map<THash, ui>>> cached_transitions;
    public:
        State() {}
        State(const vector<TElement>& pos)
        {
            this->pos = pos;
            this->hash = State::encode(pos);
        }
        static State init_from_hash(const THash& hash, const ui& radius)
        {
            State new_state;
            new_state.pos = State::decode(hash, radius);
            new_state.hash = hash;
            return new_state;
        }
        static State get_initial_state(const ui& radius)
        {
            vector<TElement> pos = vector<TElement>(radius, INVALID_POS); // all are invalid
            return State(pos);
        }
        THash get_hash() const
        {
            return this->hash;
        }
        vector<TElement> get_pos() const
        {
            return this->pos;
        }
        static THash encode(const vector<TElement>& pos)
        {
            THash h = 0;
            ui msb = 0;
            for (auto p: pos)
            {
                h |= p << msb;
                msb += ELEMENT_WIDTH_BITS;
            }
            return h;
        }
        static vector<TElement> decode(THash hash, const ui& radius)
        {
            vector<TElement> pos = vector<TElement>(radius);
            for (ui i = 0; i < radius; i++)
            {
                ui decoded_pos = hash & get_mask(ELEMENT_WIDTH_BITS);
                pos[i] = decoded_pos;
                hash >>= ELEMENT_WIDTH_BITS;
            }
            return pos;
        }
        unordered_map<THash, ui> get_next_states(const ui& radius) const
        {
            if (State::cached_transitions[radius].find(this->hash) == State::cached_transitions[radius].end())
            {
                unordered_map<THash, ui> new_transition;

                vector<TElement> new_pos = this->pos;
                for (ui i = 0; i < new_pos.size()-1; i++)
                    new_pos[i] = new_pos[i+1];
                new_pos[radius-1] = INVALID_POS; // the new element is invalid

                // iterate over all possible columns for the new queen, check the columns against positions of the other queens (which is in this->pos, not new_pos)
                ui new_queen_row = radius;
                for (ui new_queen_col = 0; new_queen_col < N; new_queen_col++)
                {
                    bool is_possible = true;
                    for (ui old_queen_row = 0; old_queen_row < this->pos.size(); old_queen_row++)
                    {
                        ui old_queen_col = this->pos[old_queen_row];
                        //cout << old_queen_row << " " << old_queen_col << " " << new_queen_row << " " << new_queen_col << endl;
                        if (old_queen_col == INVALID_POS) // don't need to check if the positions are invalid
                            continue;
                        if (!check(old_queen_row, old_queen_col, new_queen_row, new_queen_col))
                        {
                            is_possible = false;
                            break;
                        }
                    }

                    if (!(is_possible))
                        continue;
                    new_pos[radius-1] = new_queen_col;
                    State new_state = State(new_pos);
                    THash new_state_hash = new_state.get_hash();
                    if (new_transition.find(new_state_hash) == new_transition.end())
                        new_transition[new_state_hash] = 0;
                    new_transition[new_state_hash] += 1;
                }

                State::cached_transitions[radius][this->hash] = move(new_transition);
            }

            return State::cached_transitions[radius][this->hash];
        }

        string to_string() const
        {
            stringstream s;
            s << "(";
            for (auto p: this->pos)
                s << ui(p) << ",";
            s << ")";
            return s.str();
        }
};

vector<unordered_map<THash, unordered_map<THash, ui>>> State::cached_transitions = vector<unordered_map<THash, unordered_map<THash, ui>>>(N+1, unordered_map<THash, unordered_map<THash, ui>>());

ui dp(const ui& effective_radius)
{
    ui ans = 0;

    unordered_map<THash, ui> map1;
    unordered_map<THash, ui> map2;
    unordered_map<THash, ui>& prev = map1;
    unordered_map<THash, ui>& curr = map2;

    State initial_state = State::get_initial_state(effective_radius);
    THash initial_state_hash = initial_state.get_hash();
    curr[initial_state_hash] = 1;

    for (ui row = 1; row <= N; row++)
    {
        swap(prev, curr);
        curr.clear();

        for (auto prev_p: prev)
        {
            THash prev_state_hash = prev_p.first;
            ui prev_state_count = prev_p.second;
            State prev_state = State::init_from_hash(prev_state_hash, effective_radius);

            for (auto next_p: prev_state.get_next_states(effective_radius))
            {
                THash next_state_hash = next_p.first;
                ui next_state_count = next_p.second;
                State next_state = State::init_from_hash(next_state_hash, effective_radius);

//                cout << prev_state.to_string() << " (" << prev_state_count << ") ->" << next_state.to_string() << " (" << next_state_count << ")" << endl;

                if (curr.find(next_state_hash) == curr.end())
                    curr[next_state_hash] = 0;
                curr[next_state_hash] += prev_state_count * next_state_count;
            }
        }

    }

    for (auto p: curr)
    {
        ui curr_state_count = p.second;
        ans += curr_state_count;
    }

    return ans;
}

ui Q(const ui& w) // effective_radius = n - 1 - w
{
    array<ui, N> pos;
    ui effective_radius = N - 1 - w;
    ui ans = 0;
    cout << "Computing Q(" << N << ", " << w << ", effective_radius=" << effective_radius << ") ";
    if (effective_radius >= 13)
    {
        cout << "using dfs ";
        ans = dfs(0, pos, N, effective_radius);
    }
    else if (effective_radius > 0)
    {
        cout << "using dp ";
        ans = dp(effective_radius);
        //ans = dfs(0, pos, effective_radius, effective_radius);
    }
    else
    {
        ans = pow(N, N); // if the effective_radius, we can place each queen anywhere on a separate row
    }
    cout << ans << endl;
    return ans;
}

ui S()
{
    ui ans = 0;

    for (ui w = 0; w <= N - 1; w++)
        ans += Q(w);

    return ans;
}

int main()
{
    ui ans = S();
    cout << ans << endl;
    return 0;
}
