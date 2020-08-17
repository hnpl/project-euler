// Problem: https://projecteuler.net/problem=298

/*
    - For this problem, assume that "memory" is a synonym of "cache" (it is anyway).
    - Define a state as follows:
        + L memory and R memory
        + current score
    - However, the number of states as described above grows enormously after a few iterations.
    - We use a few observations to exploit the symmetry of the state space to reduce the number of states:
        + Observation 1: at all iterations, both L and R have the same number of digits in their memories.
        + Observation 2: we don't need to keep all permutations of L and R memories.
            . Even though there are 10 digits, there are only a few cases that the caches will behave differently:
                1. the called number is both in L and R
                    L memory moves the number to the front end of the queue.
                    R memory doesn't change.
                2. the called number is in L but not in R
                    L memory moves the number to the front end of the queue.
                    R memory discards the number at the back end of the queue and adds a new number to the front end of the queue.
                3. the called number is not L but it is in R
                    L memory discards the number at the back end of the queue and adds a new number to the front end of the queue.
                    R memory doesn't change.
                4. the called number is not in both L and R
                    L memory discards the number at the back end of the queue and adds a new number to the front end of the queue.
                    R memory discards the number at the back end of the queue and adds a new number to the front end of the queue.
        + Observation 3: the behaviors in observation 2 can be captured in a "relative" position of R to L.
            . relative_positions[i]: let L_memory[i] = K, then relative_position[i] is the index of K in R_memory.
            . We consider all states having the same relative_positions to be the same.
            . All states having the same relative_positions have the same behavisors described in observation 2.

    - Use Markov process and dynamic programming:
        + For each state, we can generate its next states.
        + We use a dictionary (a map) to keep the probability of each state so that we can calculate the probability of the next states.
    - 

*/
#include<iostream>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<array>
#include<numeric>
#include<limits>
#include<bitset>
#include<string>
#include<sstream>
#include<cmath>
#include<cassert>

using namespace std;

typedef uint64_t ui;

typedef uint32_t t_state_hash;

typedef int8_t t_score;

typedef uint8_t t_memory_number;

#define endl "\n"

const ui N = 50;

const ui MEMORY_CAPACITY = 5; // Memory capacity
const ui IN_L_NOT_IN_R = MEMORY_CAPACITY;
const ui EMPTY = MEMORY_CAPACITY+1;

const array<t_memory_number, 10> MASKS = {(t_memory_number)0x0000, (t_memory_number)0x0001, (t_memory_number)0x0003,
                                          (t_memory_number)0x0007, (t_memory_number)0x000F, (t_memory_number)0x001F,
                                          (t_memory_number)0x003F, (t_memory_number)0x007F, (t_memory_number)0x00FF,
                                          (t_memory_number)0x01FF};

enum t_replacement_policy { LeastRecentlyUsed = 0, Oldest = 1 };

class State
{
    private:
        t_state_hash hash;
        double probability;
        t_score score_diff;

        t_state_hash encode(const array<t_memory_number, MEMORY_CAPACITY>& L_memory, 
                            const array<t_memory_number, MEMORY_CAPACITY>& R_memory,
                            const t_score& score_diff)
        {
            t_state_hash hash = 0;
            ui msb_index = 0;

            array<t_memory_number, MEMORY_CAPACITY> relative_positions; // L relative to R
            fill(relative_positions.begin(), relative_positions.end(), EMPTY);
            for (ui idx_L = 0; idx_L < MEMORY_CAPACITY; idx_L++)
            {
                if (L_memory[idx_L] == 0)
                    break;

                ui idx_R = 0;

                for (; idx_R < MEMORY_CAPACITY; idx_R++)
                    if (R_memory[idx_R] == L_memory[idx_L])
                        break;

                if (R_memory[idx_R] == L_memory[idx_L]) // in both memories
                    relative_positions[idx_L] = idx_R;
                else// if (R_memory[idx_R] != L_memory[idx_L]) // in L but not in R
                    relative_positions[idx_L] = IN_L_NOT_IN_R;
            }

            // bit 0-14: encoding relative positions
            for (auto e: relative_positions)
            {
                hash = hash | ((t_state_hash)e << msb_index);
                msb_index += 3;
            }

            // score_diff takes bits 15-22
            int64_t score_diff_signed_64bit = score_diff;
            hash = hash | ((score_diff_signed_64bit & MASKS[8]) << msb_index);

            return hash;
        }

        void decode(array<t_memory_number, MEMORY_CAPACITY>& L_memory, array<t_memory_number, MEMORY_CAPACITY>& R_memory, t_score& score_diff)
        {
            t_state_hash encoded_state = this->hash;
            
            array<t_memory_number, MEMORY_CAPACITY> relative_positions;
            for (ui i = 0; i < MEMORY_CAPACITY; i++)
            {
                relative_positions[i] = encoded_state & MASKS[3];
                encoded_state >>= 3;
            }

            fill(L_memory.begin(), L_memory.end(), 0);
            fill(R_memory.begin(), R_memory.end(), 0);
            
            ui next_alphabet = 1;
            for (ui idx_L = 0; idx_L < MEMORY_CAPACITY; idx_L++)
            {
                if (relative_positions[idx_L] == EMPTY)
                    break;
                if (relative_positions[idx_L] != IN_L_NOT_IN_R) // in both L and R
                {
                    ui idx_R = relative_positions[idx_L];
                    L_memory[idx_L] = next_alphabet;
                    R_memory[idx_R] = next_alphabet;
                    next_alphabet += 1;
                }
                else // in L but not in R
                {
                    L_memory[idx_L] = next_alphabet;
                    next_alphabet += 1;
                }
            }

            for (ui idx_R = 0; idx_R < MEMORY_CAPACITY; idx_R++)
            {
                if (L_memory[idx_R] == 0)
                    break;
                if (R_memory[idx_R] != 0) // has been filled
                    continue;
                R_memory[idx_R] = next_alphabet;
                next_alphabet += 1;
            }

            // https://stackoverflow.com/questions/1751346/interpret-signed-as-unsigned
            // bit-preserved casting
            // https://stackoverflow.com/questions/29160220/which-way-is-better-to-get-lower-32-bits-of-a-64-bits-integer
            score_diff = static_cast<t_score>(encoded_state); // get the 8 lsb's
        }

        void shift_array_rightwards(array<t_memory_number, MEMORY_CAPACITY>& memory, ui from, ui to)
        {
            for (int i = min(int(to), int(MEMORY_CAPACITY-2)); i >= int(from); i--)
                memory[i+1] = memory[i];
        }

        // return true if the target is in cache
        // return false otherwise
        // also change the cache according to the replacement policy
        bool query_cache_replacing_least_recently_used_policy(array<t_memory_number, MEMORY_CAPACITY>& memory, const ui& target)
        {
            bool is_cache_hit = false;

            for (ui idx = 0; idx < MEMORY_CAPACITY; idx++)
            {
                t_memory_number number = memory[idx];
                if (number == target)
                {
                    if (idx > 0)
                    {
                        shift_array_rightwards(memory, 0, idx-1);
                        memory[0] = target;
                    }
                    return true;
                }
            }

            // not a cache hit
            shift_array_rightwards(memory, 0, MEMORY_CAPACITY-1);
            memory[0] = target;

            return is_cache_hit;
        }

        bool query_cache_replacing_longest_time_policy(array<t_memory_number, MEMORY_CAPACITY>& memory, const ui& target)
        {
            bool is_cache_hit = false;

            for (auto number: memory)
                if (number == target)
                    return true;

            // not a cache hit
            shift_array_rightwards(memory, 0, MEMORY_CAPACITY-1);
            memory[0] = target;
            return is_cache_hit;
        }

    public:
        State() {}

        State(const array<t_memory_number, MEMORY_CAPACITY>& L_memory,
              const array<t_memory_number, MEMORY_CAPACITY>& R_memory,
              const t_score& score_diff,
              const double& probability)
        {
            this->hash = encode(L_memory, R_memory, score_diff);
            this->score_diff = score_diff;
            this->probability = probability;
        }

        State(const ui hash, const double& probability)
        {
            this->hash = hash;
            this->score_diff = static_cast<t_score>(this->hash >> 15);
            this->probability = probability;
        }

        vector<State> get_next_states()
        {
            vector<State> next_states;

            array<t_memory_number, MEMORY_CAPACITY> L_memory;
            array<t_memory_number, MEMORY_CAPACITY> R_memory;
            t_score score_diff;

            this->decode(L_memory, R_memory, score_diff);

            for (ui next_digit = 1; next_digit <= 10; next_digit++)
            {
                array<t_memory_number, MEMORY_CAPACITY> next_L_memory = L_memory;
                array<t_memory_number, MEMORY_CAPACITY> next_R_memory = R_memory;
                t_score next_score_diff = score_diff;

                bool is_L_cache_hit = this->query_cache_replacing_least_recently_used_policy(next_L_memory, next_digit);
                bool is_R_cache_hit = this->query_cache_replacing_longest_time_policy(next_R_memory, next_digit);

                if (is_L_cache_hit && !is_R_cache_hit)
                    next_score_diff += 1;
                else if (!is_L_cache_hit && is_R_cache_hit)
                    next_score_diff -= 1;

                State next_state = State(next_L_memory, next_R_memory, next_score_diff, this->probability * 0.1);
                next_states.push_back(move(next_state));
            }

            return next_states;
        }

        double get_probability()
        {
            return this->probability;
        }

        void add_probability(double rhs)
        {
            this->probability += rhs;
        }

        t_score get_score_diff()
        {
            return this->score_diff;
        }

        t_state_hash get_hash()
        {
            return this->hash;
        }

        string describe_state()
        {
            array<t_memory_number, MEMORY_CAPACITY> L_memory;
            array<t_memory_number, MEMORY_CAPACITY> R_memory;
            t_score score_diff;

            this->decode(L_memory, R_memory, score_diff);

            stringstream stream;

            stream << "L = (";
            for (auto e: L_memory)
                stream << int(e) << ",";
            stream << "\b); R = (";
            for (auto e: R_memory)
                stream << int(e) << ",";
            stream << "\b); L-R = " << int(score_diff) << "; P = " << this->probability << ";";
            
            return stream.str();
        }

};

int main()
{
    double ans = 0.0;

    cout.precision(std::numeric_limits< double >::max_digits10);
    
    array<unordered_map<t_state_hash, State>, 2> P;

    ui curr_row = 0;
    ui prev_row = 1 - curr_row;

    // prepare the initial state
    array<t_memory_number, MEMORY_CAPACITY> initial_L_memory;
    fill(initial_L_memory.begin(), initial_L_memory.end(), 0);
    array<t_memory_number, MEMORY_CAPACITY> initial_R_memory;
    fill(initial_R_memory.begin(), initial_R_memory.end(), 0);
    State initial_state = State(initial_L_memory, initial_R_memory, 0, 1.0);
    P[curr_row][initial_state.get_hash()] = move(initial_state);

    for (ui n_iters = 1; n_iters <= N; n_iters++)
    {
        curr_row = 1 - curr_row;
        prev_row = 1 - curr_row;

        P[curr_row].clear();

        unordered_map<t_state_hash, State>& P_curr_row = P[curr_row];

        // for every previous state, check its next steps
        for (auto p: P[prev_row])
        {
            State& prev_state = p.second;

            for (auto next_state: prev_state.get_next_states())
            {
                t_state_hash next_state_hash = next_state.get_hash();

                if (P_curr_row.find(next_state_hash) == P_curr_row.end())
                    P_curr_row[next_state_hash] = move(next_state);
                else
                    P_curr_row[next_state_hash].add_probability(next_state.get_probability());
            }
        }

        //cout << n_iters << " " << P_curr_row.size() << endl;
    }

    for (auto p: P[curr_row])
    {
        State& state = p.second;
        double state_probability = state.get_probability();
        t_score state_score_diff = state.get_score_diff();
        ans += state_probability * abs(state_score_diff);
    }
    
    cout << ans << endl;

    return 0;
}


