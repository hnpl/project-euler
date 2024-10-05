// Question: https://projecteuler.net/problem=208

/*
  The idea roughly is,
    . Due to the problem state, the robot must be at the direction of 0 or 72 or 144 or 216 or 288 degrees.
    . There are 10 possible moves:
      . Moving along the arc turning clockwise starting from the direction of 0 degrees.
      . Moving along the arc turning clockwise starting from the direction of 72 degrees.
      . Moving along the arc turning clockwise starting from the direction of 144 degrees.
      . Moving along the arc turning clockwise starting from the direction of 216 degrees.
      . Moving along the arc turning clockwise starting from the direction of 288 degrees.
      . Moving along the arc turning counter-clockwise starting from the direction of 0 degrees.
      . Moving along the arc turning counter-clockwise starting from the direction of 72 degrees.
      . Moving along the arc turning counter-clockwise starting from the direction of 144 degrees.
      . Moving along the arc turning counter-clockwise starting from the direction of 216 degrees.
      . Moving along the arc turning counter-clockwise starting from the direction of 288 degrees.
    . For each direction, there are two possible moves: turning clockwise or turning counter-clockwise.
    . Note that, if the robot turns clockwise 5 times in a row, it will return to the initial position.
       Similar for consecutive counter-clockwise turns.
    . Let 'a', 'b', 'c', 'd', 'e' be the number of moves that are turning clockwise starting from the direction of 0, 72, 144, 216, 288 degrees respectively.
      Let 'f', 'g', 'h', 'i', 'j' be the number of moves that are turning counter-clockwise starting from the direction of 0, 72, 144, 216, 288 degrees respectively.
    . For the robot to return to the initial position, the sequence of moves must be able to be rearranged to form clockwise circles and counter clockwise circles (*).
      This means, a = b = c = d = e and f = g = h = i = j (**)
    . From (**), a - f = b - g = c - h = d - i = e - j.
    . The constraints (*) lead to the fact that, if the robot returns to its initial position, the robot must be at each direction equal amount of times (i.e., it must face each direction N/5 times).
      i.e., a + g == b + h == c + i == d + j == e + f == N/5.
    . Also, if the rearrangment leads to P clockwise circles and Q counter-clockwise circles, then the moves must go to the direction of k degrees by turning clockwise P amount of times, and by turning counter-clockwise Q amout of times.
      i.e., a - g == b - h == c - i == d - j == e - f == P - Q (note that P-Q could be any integer in the range [-N/5, N/5]).
    . We can define a state as (a - g; b - h; c - i; d - j; e - f; prev_move) and use state-based dynamic programming to count the number of path of length N.
      For the robot to return to its initial position, then,
        a - g == b - h == c - i == d - j == e - f (***)
    . However, for state-based dynamic programming, we don't need to count paths upto the length N (see problem208_meet_in_the_middle.cpp)
      We can count paths upto to the length N/2, then for each state S at N/2, we can generate the compatible states that are compatible to S.
      A compatible state T that is compatible to S if,
        . Assume that the prev_move of S is at degree K.
        . Note that, all states we generated so far are started from the direction of 0 degrees.
        . However, to be compatible with S, the state T must start from the direction of K degrees.
        . We can generate T by rotating S counter-clockwise by K degrees.
          This can be done by, for example, K = 144 degrees, then
            Any thing of 0 degrees in T are those of 144 degrees in S.
            Any thing of 72 degrees in T are those of 216 degrees in S.
            Any thing of 144 degrees in T are those of 288 degrees in S.
            Any thing of 216 degrees in T are those of 0 degrees in S.
            Any thing of 288 degrees in T are those of 72 degrees in S.
 */

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
