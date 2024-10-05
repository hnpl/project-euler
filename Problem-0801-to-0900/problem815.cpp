// Question: https://projecteuler.net/problem=815

// State-based DP where the state class contains the number of card types that has 0, 1, 2, and 3 cards
// have been drawn. That state also encodes the maximum number of non-empty piles seen.

#include<iostream>
#include<vector>
#include<utility>
#include<unordered_map>
#include<string>
#include<sstream>
#include<iomanip>

using namespace std;

typedef uint64_t ui;

const ui N = 60;
const ui BITS_PER_FIELDS = 8; // ceil(log2(N))

class State
{
    private:
        ui _get_hash() const
        {   
            ui hash = 0;
            ui msb = 0;
            hash |= (this->n0) << msb;
            msb += BITS_PER_FIELDS;
            hash |= (this->n1) << msb;
            msb += BITS_PER_FIELDS;
            hash |= (this->n2) << msb;
            msb += BITS_PER_FIELDS;
            hash |= (this->n3) << msb;
            msb += BITS_PER_FIELDS;
            hash |= (this->max_piles) << msb;
            msb += BITS_PER_FIELDS;
            return hash;
        }
    public:
        ui n0, n1, n2, n3, max_piles, hash;
        State(const ui& n0, const ui& n1, const ui& n2, const ui& n3, const ui& max_piles)
        {
            this->n0 = n0;
            this->n1 = n1;
            this->n2 = n2;
            this->n3 = n3;
            this->max_piles = max_piles;
            this->hash = this->_get_hash();
        }
        ui get_hash() const
        {
            return this->hash;
        }
        ui get_max_piles() const
        {
            return this->max_piles;
        }
        bool operator== (const State& rhs) const
        {
            return this->hash == rhs.hash;
        }
        vector<pair<State, double>> get_next_states() const
        {
            vector<pair<State, double>> next_states;
            next_states.reserve(4);
            const ui n0 = this->n0;
            const ui n1 = this->n1;
            const ui n2 = this->n2;
            const ui n3 = this->n3;
            
            // move n0 to n1
            if (n0 > 0)
            {
                double probability = 1.0 * 4 * n0 / (4 * n0 + 3 * n1 + 2 * n2 + n3);
                ui next_max_piles = max(this->max_piles, n1+n2+n3);
                next_states.push_back(make_pair(State(n0-1, n1+1, n2, n3, next_max_piles), probability));
            }
            if (n1 > 0)
            {
                double probability = 1.0 * 3 * n1 / (4 * n0 + 3 * n1 + 2 * n2 + n3);
                ui next_max_piles = max(this->max_piles, n1+n2+n3);
                next_states.push_back(make_pair(State(n0, n1-1, n2+1, n3, next_max_piles), probability));
            }
            if (n2 > 0)
            {
                double probability = 1.0 * 2 * n2 / (4 * n0 + 3 * n1 + 2 * n2 + n3);
                ui next_max_piles = max(this->max_piles, n1+n2+n3);
                next_states.push_back(make_pair(State(n0, n1, n2-1, n3+1, next_max_piles), probability));
            }
            if (n3 > 0)
            {
                double probability = 1.0 * 1 * n3 / (4 * n0 + 3 * n1 + 2 * n2 + n3);
                ui next_max_piles = max(this->max_piles, n1+n2+n3);
                next_states.push_back(make_pair(State(n0, n1, n2, n3-1, next_max_piles), probability));
            }
            return next_states;
        }
        static State get_initial_state()
        {
            return State(N, 0, 0, 0, 0);
        }
        string get_str() const
        {
            stringstream s;
            s << "State(" << n0 << ", " << n1 << ", " << n2 << ", " << n3 << ", max = " << max_piles << ", hash = " << hash << ")";
            return s.str();
        }
};

struct StateHash
{
    std::size_t operator()(const State& state) const
    {
        return state.get_hash();
    }
};

double solve()
{
    double ans = 0.0;

    unordered_map<State, double, StateHash> map1;
    unordered_map<State, double, StateHash> map2;
    unordered_map<State, double, StateHash>& curr_states = map1;
    unordered_map<State, double, StateHash>& prev_states = map2;

    State initial_state = State::get_initial_state();
    curr_states[initial_state] += 1.0;

    for (ui iter = 1; iter <= N*4; iter++)
    {
        swap(curr_states, prev_states);
        curr_states.clear();
        for (const auto& prev_state_pair: prev_states)
        {
            const State& prev_state = prev_state_pair.first;
            const double& prev_state_probability = prev_state_pair.second;
            for (auto& next_state_pair: prev_state.get_next_states())
            {
                //cout << prev_state.get_str() << "->" << next_state_pair.first.get_str() << endl;
                const State& next_state = next_state_pair.first;
                const double& next_state_probablity = next_state_pair.second;
                curr_states[next_state] += prev_state_probability * next_state_probablity;
                //cout << curr_states.size() << endl;
            }
        }
        cout << iter << " " << curr_states.size() << endl;
        if (iter < 9)
        {
            cout << iter << endl;
            for (auto state: curr_states)
                cout << state.first.get_str() << " " << state.second<< endl;
        }
    }

    for (const auto& state_pair: curr_states)
    {
        const State& state = state_pair.first;
        const double& state_probability = state_pair.second;
        ans += state.get_max_piles() * state_probability;
    }

    return ans;
}

int main()
{
    double ans = solve();
    cout << setprecision(11) << ans << endl;
    return 0;
}

