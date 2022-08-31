// Question: https://projecteuler.net/problem=413

#include<unordered_map>
#include<vector>
#include<iostream>
#include<array>
#include<utility>
#include<string>
#include<sstream>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

const ui N_DIGITS = 19;

constexpr ui get_mask(const ui& n)
{
    return (1ULL << n) - 1ULL;
};

class State
{
    private:
        ui hash;
        ui mod;
        inline ui count_add(const ui& curr_count, const ui& amount) const
        {
            ui ans = curr_count + amount;
            ans = min(ans, 2UL); // 0 -> 0
                                 // 1 -> 1
                                 // 2..N -> 2
            return ans;
        }

    public:
        static const ui ZERO;
        static const ui ONE;
        static const ui MORE_THAN_ONE;
        static const ui BIT_WIDTH;

        State() { this->hash = 0; this->mod = 0; }
        State(const array<ui, N_DIGITS>& count, const ui& overall_zero_count, const ui& mod)
        {
            this->hash = this->encode(count, overall_zero_count);
            this->mod = mod;
        }
        static vector<State> get_initial_states(const ui& n_digits)
        {
            vector<State> initial_states;

            for (ui first_digit = 1; first_digit <= 9; first_digit++)
            {
                array<ui, N_DIGITS> count;
                fill(count.begin(), count.end(), State::ZERO);
                count[first_digit % n_digits] = State::ONE;
                ui overall_zero_count = count[0];
                State initial_state(count, overall_zero_count, n_digits);
                initial_states.push_back(move(initial_state));
            }

            return initial_states;
        }
        static State get_invalid_state(const ui& n_digits)
        {
            array<ui, N_DIGITS> count;
            fill(count.begin(), count.end(), State::ZERO);
            ui overall_zero_count = State::MORE_THAN_ONE;
            return State(count, overall_zero_count, n_digits);
        }
        ui get_hash() const
        {
            return this->hash;
        }
        bool is_valid() const
        {
            return (this->hash >> (N_DIGITS * State::BIT_WIDTH)) != State::MORE_THAN_ONE;
        }
        bool count_zero_is_one() const
        {
            return (this->hash >> (N_DIGITS * State::BIT_WIDTH)) == State::ONE;
        }
        ui encode(const array<ui, N_DIGITS>& count, const ui& overall_zero_count) const
        {
            ui hash = 0;
            ui msb = 0;
            for (ui i = 0; i < N_DIGITS; i++)
            {
                hash |= (count[i] << msb);
                msb += State::BIT_WIDTH;
            }
            hash |= (overall_zero_count << msb);
            return hash;
        }
        pair<array<ui, N_DIGITS>, ui> decode() const
        {
            array<ui, N_DIGITS> count;
            fill(count.begin(), count.end(), 0);
            ui overall_zero_count = State::ZERO;
            ui hash = this->hash;
            for (ui i = 0; i < N_DIGITS; i++)
            {
                count[i] = hash & get_mask(State::BIT_WIDTH);
                hash >>= State::BIT_WIDTH;
            } 
            overall_zero_count = hash & get_mask(State::BIT_WIDTH);
            return make_pair(count, overall_zero_count);
        }
        void reset() // zero-out count, state is invalid
        {
            this->hash = 0;
        }
        vector<State> get_next_states() const
        {
            vector<State> next_states;
            next_states.reserve(10);

            auto p = this->decode();
            auto curr_count = p.first;
            auto curr_overall_zero_count = p.second;

            for (ui next_digit = 0; next_digit < 10; next_digit++)
            {
                array<ui, N_DIGITS> next_count;
                ui next_overall_zero_count = State::ZERO;

                fill(next_count.begin(), next_count.end(), State::ZERO);
                ui next_digit_remainder = next_digit % this->mod;
                next_count[next_digit_remainder] = this->count_add(next_count[next_digit_remainder], State::ONE);
                for (ui curr_remainder = 0; curr_remainder < this->mod; curr_remainder++)
                {
                    ui next_remainder = ((curr_remainder * 10) + next_digit) % this->mod;
                    next_count[next_remainder] = this->count_add(next_count[next_remainder], curr_count[curr_remainder]);
                    //cout << "curr_remainder: " << curr_remainder << "; digit: " << next_digit << "; next_remainder: " << next_remainder << endl; 
                }
                next_overall_zero_count = this->count_add(next_count[0], curr_overall_zero_count);

                State next_state;
                if (next_overall_zero_count == State::MORE_THAN_ONE)
                {
                    //next_state = State(next_count, next_overall_zero_count, this->mod);
                }
                else
                {
                    next_state = State(next_count, next_overall_zero_count, this->mod);
                    next_states.push_back(move(next_state));
                }
            }

            return next_states;
        }
        string to_string() const
        {
            auto p = this->decode();
            auto count = p.first;
            auto overall_zero_count = p.second;
            stringstream s;
            s << "[";
            for (auto c: count)
                cout << c << ", ";
            s << "]; Overall zero count: ";
            s << overall_zero_count;
            return s.str();
        }
};

ui const State::ZERO = 0;
ui const State::ONE = 1;
ui const State::MORE_THAN_ONE = 2;
ui const State::BIT_WIDTH = 2;

ui count_one_child_numbers(const ui& n_digits)
{
    auto initial_states = State::get_initial_states(n_digits);

    ui ans = 0;

    unordered_map<ui, State> hash_state_map;
    unordered_map<ui, ui> hash_count_1;
    unordered_map<ui, ui> hash_count_2;
    unordered_map<ui, ui>& curr = hash_count_1;
    unordered_map<ui, ui>& prev = hash_count_2;


    for (auto initial_state: initial_states)
    {
        auto initial_state_hash = initial_state.get_hash();
        if (curr.find(initial_state_hash) == curr.end())
            curr[initial_state_hash] = 0;
        curr[initial_state_hash] += 1;
        if (hash_state_map.find(initial_state_hash) == hash_state_map.end())
            hash_state_map[initial_state_hash] = initial_state;
    }

    for (ui i = 1; i < n_digits; i++)
    {
        swap(curr, prev);
        curr.clear();
        
        for (auto prev_pair: prev)
        {
            auto prev_state_hash = prev_pair.first;
            auto prev_state_count = prev_pair.second;
            auto prev_state = hash_state_map[prev_state_hash];
            //cout <<prev_state.to_string() << endl;
            if (!prev_state.is_valid())
                continue;
            for (auto next_state: prev_state.get_next_states())
            {
                //cout << "-> " << next_state.to_string() << endl;
                auto next_state_hash = next_state.get_hash();
                if (hash_state_map.find(next_state_hash) == hash_state_map.end())
                    hash_state_map[next_state_hash] = next_state;
                ui next_state_count = prev_state_count;
                if (curr.find(next_state_hash) == curr.end())
                    curr[next_state_hash] = 0;
                curr[next_state_hash] += next_state_count;
            }
        }
    }

    cout << hash_state_map.size() << endl;

    for (auto state_hash_count: curr)
    {
        auto state_hash = state_hash_count.first;
        auto count = state_hash_count.second;
        auto state = hash_state_map[state_hash];
        //cout << state.to_string() << "; Freq = " << count << endl;
        if (state.count_zero_is_one())
            ans += count;
    }

    return ans;
}

ui F(const ui& max_n_digits)
{
    ui ans = 0;
    for (ui n_digits = 1; n_digits <= max_n_digits; n_digits++)
    {
        ans += count_one_child_numbers(n_digits);
        cout << "F(10**" <<n_digits<<")=" << ans << endl;
    }
    return ans;
}

int main()
{
    ui ans = F(N_DIGITS);
    cout << ans << endl;
    return 0;
}
