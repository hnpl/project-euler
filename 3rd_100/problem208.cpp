// Question: https://projecteuler.net/problem=208

#include<iostream>
#include<vector>
#include<array>
#include<unordered_map>
#include<cmath>
#include<algorithm>
#include<utility>
#include<sstream>

using namespace std;

typedef int64_t ui;
#define endl "\n"

//const ui N = 5;
//const ui N = 25;
const ui N = 70;

constexpr ui get_mask(const ui& n)
{
    return (1ULL << n) - 1ULL;
};

enum Move
{
    CLOCKWISE_FROM_0 = 0,
    CLOCKWISE_FROM_72 = 1,
    CLOCKWISE_FROM_144 = 2,
    CLOCKWISE_FROM_216 = 3,
    CLOCKWISE_FROM_288 = 4,
    COUNTER_CLOCKWISE_FROM_0 = 5,
    COUNTER_CLOCKWISE_FROM_72 = 6,
    COUNTER_CLOCKWISE_FROM_144 = 7,
    COUNTER_CLOCKWISE_FROM_216 = 8,
    COUNTER_CLOCKWISE_FROM_288 = 9,
    INVALID = 10
};

enum Direction
{
    AT_0 = 0,
    AT_72 = 1,
    AT_144 = 2,
    AT_216 = 3,
    AT_288 = 4
};
                                        //AT_0, AT_72, AT_144, AT_216, AT_288
const vector<vector<ui>> contribution = {{0   ,     1, 0     , 0     , 0     },  // CLOCKWISE_FROM_0
                                         {0   , 0    ,      1, 0     , 0     },  // CLOCKWISE_FROM_72
                                         {0   , 0    , 0     ,      1, 0     },  // CLOCKWISE_FROM_144
                                         {0   , 0    , 0     , 0     ,      1},  // CLOCKWISE_FROM_216
                                         {   1, 0    , 0     , 0     , 0     },  // CLOCKWISE_FROM_288
                                         {0   , 0    , 0     , 0     ,      1},  // COUNTER_CLOCKWISE_FROM_0
                                         {   1, 0    , 0     , 0     , 0     },  // COUNTER_CLOCKWISE_FROM_72
                                         {0   ,     1, 0     , 0     , 0     },  // COUNTER_CLOCKWISE_FROM_144
                                         {0   , 0    ,      1, 0     , 0     },  // COUNTER_CLOCKWISE_FROM_216
                                         {0   , 0    , 0     ,      1, 0     }}; // COUNTER_CLOCKWISE_FROM_288


const vector<Direction> move_destination_map = {  AT_72,  // CLOCKWISE_FROM_0
                                                 AT_144,  // CLOCKWISE_FROM_72
                                                 AT_216,  // CLOCKWISE_FROM_144
                                                 AT_288,  // CLOCKWISE_FROM_216
                                                   AT_0,  // CLOCKWISE_FROM_288
                                                 AT_288,  // COUNTER_CLOCKWISE_FROM_0
                                                   AT_0,  // COUNTER_CLOCKWISE_FROM_72
                                                  AT_72,  // COUNTER_CLOCKWISE_FROM_144
                                                 AT_144,  // COUNTER_CLOCKWISE_FROM_216
                                                 AT_216}; // COUNTER_CLOCKWISE_FROM_288

const vector<array<Move, 2>> next_moves_map = { { Move::CLOCKWISE_FROM_72, Move::COUNTER_CLOCKWISE_FROM_72 },
                                                { Move::CLOCKWISE_FROM_144, Move::COUNTER_CLOCKWISE_FROM_144 },
                                                { Move::CLOCKWISE_FROM_216, Move::COUNTER_CLOCKWISE_FROM_216 },
                                                { Move::CLOCKWISE_FROM_288, Move::COUNTER_CLOCKWISE_FROM_288 },
                                                { Move::CLOCKWISE_FROM_0, Move::COUNTER_CLOCKWISE_FROM_0 },
                                                { Move::CLOCKWISE_FROM_288, Move::COUNTER_CLOCKWISE_FROM_288 },
                                                { Move::CLOCKWISE_FROM_0, Move::COUNTER_CLOCKWISE_FROM_0 },
                                                { Move::CLOCKWISE_FROM_72, Move::COUNTER_CLOCKWISE_FROM_72 },
                                                { Move::CLOCKWISE_FROM_144, Move::COUNTER_CLOCKWISE_FROM_144 },
                                                { Move::CLOCKWISE_FROM_216, Move::COUNTER_CLOCKWISE_FROM_216}
                                              };

array<Move, 2> get_next_moves(const Move& curr_move)
{
    return next_moves_map[curr_move];
};

constexpr bool is_counter_clockwise_move(const Move& curr_move)
{
    return curr_move >= 5;
};

class State
{
    private:
        ui hash;
        ui prev_move;
        static const ui DIFF_WIDTH;
        static const ui MOVE_WIDTH;
    public:
        State()
        {
            this->hash = 0;
            this->prev_move = 0;
        }
        State(const array<int8_t, 5>& diff, const ui& prev_move)
        {
            this->hash = this->encode(diff, prev_move);
        }
        ui encode(const array<int8_t, 5>& diff, const ui& prev_move) const
        {
            ui msb = 0;
            ui hash = 0;
            for (ui i = 0; i < diff.size(); i++)
            {
                hash |= ((ui)(diff[i] & get_mask(State::DIFF_WIDTH))) << msb;
                msb += State::DIFF_WIDTH;
            }
            hash |= prev_move << msb;
            return hash;
        }
        bool is_result_state() const
        {
            auto p = this->decode();
            auto diff = p.first;
            ui pivot = diff[0];
            for (auto d: diff)
                if (d != pivot)
                    return false;
            return true;
        }
        pair<array<int8_t, 5>, ui> decode() const
        {
            array<int8_t, 5> diff;
            fill(diff.begin(), diff.end(), 0);
            ui prev_move = Move::INVALID;

            ui hash = this->hash;
            for (ui i = 0; i < diff.size(); i++)
            {
                diff[i] = (int8_t)(hash & get_mask(State::DIFF_WIDTH));
                hash >>= State::DIFF_WIDTH;
            }
            prev_move = (Move)(hash & get_mask(State::MOVE_WIDTH));
            return make_pair(diff, prev_move);
        }
        ui get_hash() const
        {
            return this->hash;
        }
        vector<State> get_next_states(const ui& max_diff) const
        {
            vector<State> next_states;
            next_states.reserve(2);
            auto curr_state = this->decode();
            auto curr_diff = curr_state.first;
            Move curr_move = Move(curr_state.second);
            auto next_moves = get_next_moves(curr_move);
            for (auto next_move: next_moves)
            {
                array<int8_t, 5> next_diff = curr_diff;
                /*
                ui move_idx = next_move % 5;
                if (is_counter_clockwise_move(next_move))
                    next_diff[move_idx] -= 1;
                else
                    next_diff[move_idx] += 1;
                if (abs(next_diff[move_idx]) > N/5)
                    continue;
                */
                Direction new_direction = move_destination_map[next_move];
                if (is_counter_clockwise_move(next_move))
                {
                    next_diff[new_direction] -= 1;
                    if (abs(next_diff[new_direction]) > max_diff)
                        continue;
                }
                else
                {
                    next_diff[new_direction] += 1;
                    if (abs(next_diff[new_direction]) > max_diff)
                        continue;
                }
                next_states.emplace_back(next_diff, next_move);
            }
            return next_states;
        }
        static State get_initial_state()
        {
            array<int8_t, 5> diff = { 0, 0, 0, 0, 0 };
            Move prev_move = Move::CLOCKWISE_FROM_288;
            return State(diff, prev_move);
        }
        string to_string() const
        {
            auto p = this->decode();
            auto diff = p.first;
            auto prev_move = p.second;
            stringstream s;
            s << "diff = [" << (ui)diff[0] << ", " << (ui)diff[1] << ", " << (ui)diff[2] << ", " << (ui)diff[3] << ", " << (ui)diff[4];
            s << "]; prev_move = " << prev_move;
            return s.str();
        }
};
const ui State::DIFF_WIDTH = 8;
const ui State::MOVE_WIDTH = 4;

ui count_paths(ui n)
{
    ui ans = 0;

    unordered_map<ui, State> hash_state_map;
    unordered_map<ui, ui> hash_count_1;
    unordered_map<ui, ui> hash_count_2;
    unordered_map<ui, ui>& prev = hash_count_1;
    unordered_map<ui, ui>& curr = hash_count_2;

    State initial_state = State::get_initial_state();
    ui initial_state_hash = initial_state.get_hash();
    hash_state_map[initial_state_hash] = initial_state;
    curr[initial_state_hash] = 1;

    for (ui i = 0; i < n; i++)
    {
        cout << i << " " << curr.size() << endl;
        swap(curr, prev);
        curr.clear();

        for (auto prev_state_p: prev)
        {
            ui prev_state_hash = prev_state_p.first;
            ui prev_state_count = prev_state_p.second;
            auto prev_state = hash_state_map[prev_state_hash];
            for (auto next_state: prev_state.get_next_states(n/5))
            {
                ui next_state_hash = next_state.get_hash();
                if (hash_state_map.find(next_state_hash) == hash_state_map.end())
                    hash_state_map[next_state_hash] = next_state;
                if (curr.find(next_state_hash) == curr.end())
                    curr[next_state_hash] = 0;
                curr[next_state_hash] += prev_state_count;
            }
        }
    }

    for (auto curr_state_p: curr)
    {
        auto curr_state_hash = curr_state_p.first;
        auto curr_state_count = curr_state_p.second;
        auto curr_state = hash_state_map[curr_state_hash];
        if (curr_state.is_result_state())
        {
            cout << "ans: " << curr_state.to_string() << " count = " << curr_state_count << endl;
            ans += curr_state_count;
        }
    }

    return ans;
}

int main()
{
    ui ans = count_paths(N);
    cout << ans << endl;
    return 0;
}
