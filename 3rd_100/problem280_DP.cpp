// Problem: https://projecteuler.net/problem=280

/*
    . Use bits to encode states.
    . Calculate expectation using probability of reaching the stopping state by iterating through N number of steps, and 
    for each iteration, we discover next states of all current states and calculate their probabilities.
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

using namespace std;

typedef uint64_t ui;

typedef uint32_t t_state_hash;

#define endl "\n"

const ui N = 5;

const ui MOST_UPPER_ROW = N - 1;

const ui MOST_LOWER_ROW = 0;

const array<ui, 10> MASKS = {0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
                             0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF};

inline void position_to_x_y(ui position, ui& x, ui& y)
{
    x = position / N;
    y = position % N;
}

inline void x_y_to_position(ui x, ui y, ui& position)
{
    position = x * N + y;
}

inline ui x_y_to_position(ui x, ui y)
{
    return x * N + y;
}

inline bool is_valid_x_y(ui x, ui y)
{
    return (x >= 0) && (x < N) && (y >= 0) && (y < N);
}


struct State
{
    t_state_hash hash;
    double probability;

    //bool n_seeds_in_row5_initialized;
    ui n_seeds_in_row5;

    State() { }

    State(ui                    _is_carrying_seed,
          ui                    _position_x,
          ui                    _position_y,
          const array<ui, N>&   _row1_n_seeds,
          const array<ui, N>&   _row5_n_seeds)
    {
        hash = encode(_is_carrying_seed, x_y_to_position(_position_x, _position_y), _row1_n_seeds, _row5_n_seeds);
        probability = 1.0;
        //n_seeds_in_row5_initialized = false;
        n_seeds_in_row5 = accumulate(_row5_n_seeds.begin(), _row5_n_seeds.end(), 0ULL);
    }

    string describe_state()
    {
        ui is_carrying_seed;
        ui position_x;
        ui position_y;
        array<ui, N> row1_n_seeds;
        array<ui, N> row5_n_seeds;
        t_state_hash encoded_state = hash;
        
        // bit 0: is_carrying_seed
        is_carrying_seed = encoded_state & MASKS[1];
        encoded_state >>= 1;

        // bit 1-5: ant's position
        ui position = encoded_state & MASKS[5];
        position_to_x_y(position, position_x, position_y);
        encoded_state >>= 5;

        // bit 6-10: bit[i] == 1 means grid[0][i-6] has a seed, 0 otherwise
        for (ui i = 0; i < 5; i++)
        {
            row1_n_seeds[i] = encoded_state & MASKS[1];
            encoded_state >>= 1;
        }

        // bit 11-25: bits[11..13] == n_seeds in grid[N-1][0]
        //            bits[14..16] == n_seeds in grid[N-1][1]
        //            and so on
        for (ui i = 0; i < 5; i++)
        {
            row5_n_seeds[i] = encoded_state & MASKS[3];
            encoded_state >>= 3;
        }

        stringstream s;
        s << "position: ("<<position_x <<"," << position_y<<"); is_carrying_seed=" << is_carrying_seed << "; row1_n_seeds (";
        for (auto i: row1_n_seeds)
            s <<i <<",";

        s <<"\b); row5_n_seeds (";
        for (auto i: row5_n_seeds)
            s <<i <<",";


        s <<")"<< endl;

        return s.str();
    }

    void decode(ui& is_carrying_seed, ui& position_x, ui& position_y, array<ui, N>& row1_n_seeds, array<ui, N>& row5_n_seeds)
    {
        t_state_hash encoded_state = hash;

        // bit 0: is_carrying_seed
        is_carrying_seed = encoded_state & MASKS[1];
        encoded_state >>= 1;

        // bit 1-5: ant's position
        ui position = encoded_state & MASKS[5];
        position_to_x_y(position, position_x, position_y);
        encoded_state >>= 5;

        // bit 6-10: bit[i] == 1 means grid[0][i-6] has a seed, 0 otherwise
        for (ui i = 0; i < N; i++)
        {
            row1_n_seeds[i] = encoded_state & MASKS[1];
            encoded_state >>= 1;
        }

        // bit 11-25: bits[11..13] == n_seeds in grid[N-1][0]
        //            bits[14..16] == n_seeds in grid[N-1][1]
        //            and so on
        for (ui i = 0; i < N; i++)
        {
            row5_n_seeds[i] = encoded_state & MASKS[3];
            encoded_state >>= 3;
        } 
    }

    t_state_hash encode(ui is_carrying_seed, ui position, const array<ui, N>& row1_n_seeds, const array<ui, N>& row5_n_seeds)
    {
        t_state_hash msb_index = 0;
        t_state_hash _hash = 0;
        
        // bit 0: is_carrying_seed
        _hash = _hash | (is_carrying_seed);
        msb_index = 1;

        // bit 1-5: ant's position
        _hash = _hash | (position << msb_index);
        msb_index += 5;

        // bit 6-10: bit[i] == 1 means grid[0][i-6] has a seed, 0 otherwise
        for (ui i = 0; i < N; i++)
        {
            _hash = _hash | (row1_n_seeds[i] << msb_index);
            msb_index += 1;
        }

        // bit 11-25: bits[11..13] == n_seeds in grid[N-1][0]
        //            bits[14..16] == n_seeds in grid[N-1][1]
        //            and so on
        for (ui i = 0; i < N; i++)
        {
            _hash = _hash | (row5_n_seeds[i] << msb_index);
            msb_index += 3;
        }

        return _hash;
    }

    // try to move to all of its neighbor, return the next states
    vector<State> get_next_states()
    {
        vector<State> next_states;

        // decode the hash
        ui is_carrying_seed;
        ui position_x;
        ui position_y;
        array<ui, N> row1_n_seeds;
        array<ui, N> row5_n_seeds;
        decode(is_carrying_seed, position_x, position_y, row1_n_seeds, row5_n_seeds);


        // move to each of its neighbors
        vector<ui> neighbors_positions = get_neighbors_positions(position_x, position_y);
        ui n_neighbors = neighbors_positions.size();
        for (auto neighbor_position: neighbors_positions)
        {
            ui next_position_x;
            ui next_position_y;
            position_to_x_y(neighbor_position, next_position_x, next_position_y);

            int collected_seed_x = -1;
            int dropped_seed_x = -1;

            ui next_is_carrying_seed = is_carrying_seed;
            if (is_carrying_seed && (next_position_y == MOST_UPPER_ROW) && (row5_n_seeds[next_position_x] == 0))
            {
                row5_n_seeds[next_position_x] = 1;
                next_is_carrying_seed = 0;
                dropped_seed_x = next_position_x;
            }

            if (!is_carrying_seed && (next_position_y == MOST_LOWER_ROW) && (row1_n_seeds[next_position_x] == 1))
            {
                row1_n_seeds[next_position_x] = 0;
                next_is_carrying_seed = 1;
                collected_seed_x = next_position_x;
            }

            State next_state = State(next_is_carrying_seed,
                                     next_position_x,
                                     next_position_y,
                                     row1_n_seeds,
                                     row5_n_seeds);
            next_state.probability = probability * (1.0 / n_neighbors);
            next_states.push_back(next_state);

            // Restore the arrays
            if (dropped_seed_x >= 0)
                row5_n_seeds[dropped_seed_x] = 0;
            if (collected_seed_x >= 0)
                row1_n_seeds[collected_seed_x] = 1;

        }
        return next_states;
    }

    bool is_stopping_state()
    {
        return n_seeds_in_row5 == N;
    }

    t_state_hash get_hash()
    {
        return hash;
    }

    vector<ui> get_neighbors_positions(ui position_x, ui position_y)
    {
        vector<ui> neighbors;

        int x, dx, fx;
        int y, dy, fy;
        x = int(position_x);
        y = int(position_y);

        // up
        dx = 0; dy = 1;
        fx = x + dx; fy = y + dy;
        if (is_valid_x_y(fx, fy))
            neighbors.push_back(x_y_to_position(fx, fy));
        
        // down
        dx = 0; dy = -1;
        fx = x + dx; fy = y + dy;
        if (is_valid_x_y(fx, fy))
            neighbors.push_back(x_y_to_position(fx, fy));

        // left
        dx = -1; dy = 0;
        fx = x + dx; fy = y + dy;
        if (is_valid_x_y(fx, fy))
            neighbors.push_back(x_y_to_position(fx, fy));

        // right
        dx = 1; dy = 0;
        fx = x + dx; fy = y + dy;
        if (is_valid_x_y(fx, fy))
            neighbors.push_back(x_y_to_position(fx, fy));

        return neighbors;
    }

};

int main()
{
    double ans;


    cout.precision(std::numeric_limits< double >::max_digits10);
    
    array<unordered_map<t_state_hash, State>, 2> P;

    ui curr_row = 0;
    ui prev_row = 1 - curr_row;

    array<ui, N> initial_row1;
    fill(initial_row1.begin(), initial_row1.end(), 1);
    array<ui, N> initial_row5;
    fill(initial_row5.begin(), initial_row5.end(), 0);
    State initial_state = State(0, 2, 2, initial_row1, initial_row5);

    P[curr_row][initial_state.get_hash()] = initial_state;

    double prev_probability = 0.0;
    double expectation = 0.0;
    double prev_expectation = 1.0;

    double tol = 1e-12;

    ui n_steps = 0;

    //for (ui n_steps = 1; n_steps <= 10000; n_steps++)
    while ((abs(expectation - prev_expectation) > tol) || (prev_probability < tol))
    {
        n_steps = n_steps + 1;

        curr_row = 1 - curr_row;
        prev_row = 1 - curr_row;

        prev_expectation = expectation;

        P[curr_row].clear();

        for (auto p: P[prev_row])
        {
            State prev_state = p.second;
            if (prev_state.is_stopping_state())
            {
                if (P[curr_row].find(prev_state.get_hash()) == P[curr_row].end())
                    P[curr_row][prev_state.get_hash()] = prev_state;
                else
                    P[curr_row][prev_state.get_hash()].probability += prev_state.probability;
            }
            else
            {
                vector<State> next_states = prev_state.get_next_states();
                for (auto next_state: next_states)
                {
                    ui next_state_hash = next_state.get_hash();
                    if (P[curr_row].find(next_state_hash) == P[curr_row].end())
                        P[curr_row][next_state_hash] = next_state;
                    else
                        P[curr_row][next_state_hash].probability += next_state.probability;
                }
            }
        }

        double curr_probability = 0.0;
        for (auto p: P[curr_row])
        {
            State state = p.second;
            if (state.is_stopping_state())
                curr_probability = curr_probability + state.probability;
        }
        expectation += n_steps * (curr_probability - prev_probability);
        prev_probability = curr_probability;
        cout << n_steps << " " << P[curr_row].size() << " " << expectation << endl;
    }

    ans = expectation;
  
    cout << ans << endl;

    return 0;
}


