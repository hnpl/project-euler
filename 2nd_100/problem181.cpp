// Question: https://projecteuler.net/problem=181

/*
 * . The idea is to build the group by adding the partitions one-by-one such that, the next partition either,
 *     . More black objects and any number of white objects.
 *     . Equal number of black objects and more white objects than the previous partition.
 * . Since the partition can be repeated, we add one or more partitions of the same type (same number of black objects and same number white objects) to the group we are building.
 * . Essentially, we are enforcing the order of the partition: from left to right, the number of of black objects increases, and if two adjacent objects have the same number of black objects,
 *   the group on the right has equal or more number of white objects.
 */

#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<utility>
#include<array>

using namespace std;

typedef uint64_t ui;
typedef uint32_t THash;
typedef uint8_t TCount; // for counting the number of objects
#define endl "\n"

const ui MAX_B = 60;
const ui MAX_W = 40;
const ui BIT_WIDTH = 8;

constexpr ui get_mask(const ui& n)
{
    return (1 << n) - 1;
};

class State
{
    private:
        TCount prev_B;
        TCount prev_W;
        TCount n_B;
        TCount n_W;
        THash hash;
        static const THash result_state_mask;
    public:
        State() : prev_B(0), prev_W(0), n_B(0), n_W(0), hash(0) {}
        State(const TCount& prev_B, const TCount& prev_W, const TCount& n_B, const TCount& n_W)
            : prev_B(prev_B), prev_W(prev_W), n_B(n_B), n_W(n_W)
        {
            this->hash = this->encode(prev_B, prev_W, n_B, n_W);
        }
        static State init_from_hash(const ui& hash)
        {
            State new_state;
            auto decoded_hash = State::decode(hash);
            new_state.prev_B = decoded_hash[0];
            new_state.prev_W = decoded_hash[1];
            new_state.n_B = decoded_hash[2];
            new_state.n_W = decoded_hash[3];
            return new_state;
        }
        THash encode(const TCount& prev_B, const TCount& prev_W, const TCount& n_B, const TCount& n_W) const
        {
            THash hash = 0;
            ui msb = 0;
            hash |= (prev_B << msb);
            msb += BIT_WIDTH;
            hash |= (prev_W << msb);
            msb += BIT_WIDTH;
            hash |= (n_B << msb);
            msb += BIT_WIDTH;
            hash |= (n_W << msb);
            msb += BIT_WIDTH;
            return hash;
        }

        static array<TCount, 4> decode(THash hash)
        {
            array<TCount, 4> ans = { 0, 0, 0, 0 };
            for (ui i = 0; i < 4; i++)
            {
                ans[i] = hash & get_mask(BIT_WIDTH);
                hash >>= BIT_WIDTH;
            }
            return ans;
        }

        TCount get_prev_B() const { return this->prev_B; }
        TCount get_prev_W() const { return this->prev_W; }
        TCount get_n_B() const { return this->n_B; }
        TCount get_n_W() const { return this->n_W; }
        THash get_hash() const { return this->hash; }

        bool is_compatible(const TCount& other_B, const TCount& other_W, const TCount& max_B, const TCount& max_W) const // can we add this state and the other state?
        {
            if (((this->n_B + other_B) > max_B) || ((this->n_W + other_W) > max_W))
                return false;
            if (this->prev_B == other_B)
                return this->prev_W < other_W;
            else if (this->prev_B > other_B)
                return false;
            else
                return true;
            return true;
        }

        bool is_result_state(const TCount& max_B, const TCount& max_W) const
        {
            if ((this->n_B == max_B) && (this->n_W == max_W))
                return true;
            return false;
        }
        bool is_result_state() const
        {
            return (this->hash & State::result_state_mask) == State::result_state_mask;
        }
        bool has_next_states(const TCount& max_B, const TCount& max_W) const // at some point, we have to add one more partition such that the number of B > prev_B to make n_B == max_B
        {
            return (this->n_B + this->prev_B) <= max_B;
        }
        vector<THash> get_next_states(const ui& max_B, const ui& max_W) const
        {
            vector<THash> next_states;

            // next_B = prev_B, next_W > prev_W
            for (TCount next_W = this->prev_W + 1; this->n_W + next_W <= max_W; next_W++)
            {
                TCount next_B = this->prev_B;
                for (TCount n_repeats = 1; is_compatible(n_repeats * next_B, n_repeats * next_W, max_B, max_W); n_repeats++)
                {
                    TCount next_n_B = n_B + n_repeats * next_B;
                    TCount next_n_W = n_W + n_repeats * next_W;
                    State next_state = State(next_B, next_W, next_n_B, next_n_W);
                    if (next_state.has_next_states(max_B, max_W) || next_state.is_result_state(max_B, max_W))
                        next_states.push_back(move(next_state.get_hash()));
                }
            }

            // next_B > prev_B, next_W in [0..max_W-n_W]
            for (TCount next_B = this->prev_B + 1; this->n_B + next_B <= max_B; next_B++)
            {
                for (TCount next_W = 0; this->n_W + next_W <= max_W; next_W++)
                {
                    for (TCount n_repeats = 1; is_compatible(n_repeats * next_B, n_repeats * next_W, max_B, max_W); n_repeats++)
                    {
                        TCount next_n_B = this->n_B + n_repeats * next_B;
                        TCount next_n_W = this->n_W + n_repeats * next_W;
                        State next_state = State(next_B, next_W, next_n_B, next_n_W);
                        if (next_state.has_next_states(max_B, max_W) || next_state.is_result_state(max_B, max_W))
                            next_states.push_back(move(next_state.get_hash()));
//                        else
//                            cout << "Rejected " << next_state.to_string() << endl;
                    }
                }
            }

            return next_states;
        }

        static State get_initial_state()
        {
            return State();
        }
        string to_string() const
        {
            stringstream s;

            s << "prev_B: " << (ui)this->prev_B << "; prev_W: " << (ui)this->prev_W << "; n_B: " << (ui)this->n_B << "; n_W: " << (ui)this->n_W;

            return s.str();
        }
};

const THash State::result_state_mask = (MAX_B << (BIT_WIDTH*2)) | (MAX_W << (BIT_WIDTH*3));

ui partition(const TCount& max_B, const TCount& max_W)
{
    ui ans = 0;
    TCount n_partitions_max = max_B + max_W;

//    unordered_map<THash, State> hash_state_map;
    unordered_map<THash, ui> map_1;
    unordered_map<THash, ui> map_2;
    unordered_map<THash, ui>& prev = map_1;
    unordered_map<THash, ui>& curr = map_2;

    State initial_state = State::get_initial_state();
    THash initial_state_hash = initial_state.get_hash();
    curr[initial_state_hash] = 1;

    for (TCount n_partitions = 1; n_partitions <= n_partitions_max; n_partitions++)
    {
        cout << (ui)n_partitions << " " << curr.size() << " " << ans<< endl;
        swap(prev, curr);
        curr.clear();
        for (auto p: prev)
        {
            THash prev_state_hash = p.first;
            // State prev_state = hash_state_map[prev_state_hash];
            State prev_state = State::init_from_hash(prev_state_hash);
            ui prev_count = p.second;

            for (auto next_state_hash: prev_state.get_next_states(max_B, max_W))
            {
//                State next_state = State::init_from_hash(next_state_hash);
//                cout << "(" << prev_state.to_string() << ") -> (" << next_state.to_string() << ")";
//                if (next_state.is_result_state(max_B, max_W))
//                    cout << " *";
//                cout << endl;
                if (curr.find(next_state_hash) == curr.end())
                    curr[next_state_hash] = 0;
                curr[next_state_hash] += prev_count;
            }
        }

        for (auto c: curr)
        {
            THash curr_state_hash = c.first;
            // State curr_state = hash_state_map[curr_state_hash];
            State curr_state = State::init_from_hash(curr_state_hash);
            ui curr_state_count = c.second;

            if (curr_state.is_result_state(max_B, max_W))
                ans += curr_state_count;
        }
    }

    return ans;
}

int main()
{
    ui ans = partition(MAX_B, MAX_W);
    cout << ans << endl;
    return 0;
}
