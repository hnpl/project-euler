// Problem: https://projecteuler.net/problem=161

/*
    . Starting from a blank board, try placing a shape on the first square on the top row that is not completely filled.
    . Use DFS to search for all possible shape placements.
    . Use memoization to cache results.
    . Encode the number of filled rows and the next 3 rows as a hash to query the cached results.
      We only new the next 3 rows because at most, a shape will occupy squares spanning 3 rows.
    . Note that this shape requires attentions as square(0,0) is not occupied.
      Shape:  *
             **
      Failing to pay attention to this case leads to incorrect results.
*/

#include<iostream>
#include<bitset>
#include<vector>
#include<array>
#include<unordered_map>
#include<unordered_set>
#include<string>
#include<algorithm>
#include<sstream>
#include<cassert>

using namespace std;

#define endl "\n"

typedef __uint128_t THash;
typedef __uint128_t ui;

const ui N = 12;
const ui M = 9;

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

constexpr THash mask(ui n_ones)
{
    if (n_ones == 1)
        return 1;
    return (mask(n_ones-1) << 1) | THash(1);
};

class Shape
{
    public:
        THash hash;
        ui most_significant_one_index;
        Shape() { this->hash = 0; this->most_significant_one_index = 0; }
        void add_tile(ui col, ui row)
        {
            this->hash = this->hash | ((THash)1 << (row * (M+1) + col));
            this->most_significant_one_index = max(this->most_significant_one_index, (row * (M+1) + col));
        }
        THash get_hash() const
        {
            return this->hash;
        }
};

class State
{
    private:
        ui height;
        ui width;

        // bits[0..4]: n_filled_rows
        // bits[5..] : next 3 rows
        void encode(THash& grid_hash, ui& n_filled_rows)
        {
            THash hash = 0;

            ui h = min((ui)3, N - n_filled_rows);
            ui w = M+1;

            ui msb_index = 5;

            // placing the fences
            for (ui row = 0; row < 3; row++)
                hash = hash | (((THash)1) << ((w-1) + (w*row) + msb_index));

            bool prev_filled = true;

            for (ui i = 0; i < h; i++)
            {
                // if the row is completely filled, increase n_filled_rows and skip to the next row
                if (((grid_hash & mask(w)) == mask(w)) && prev_filled)
                {
                    n_filled_rows += 1;
                    grid_hash >>= w;
                    continue;
                }

                // fill the row
                hash = hash | ((grid_hash & mask(w)) << msb_index);
                msb_index += w;
                grid_hash >>= w;
                prev_filled = false;
            }

            // fill the extra rows (i.e., if we have N rows, then row N+1 and row N+2 and so on rows must be filled)
            msb_index = 5;
            for (ui row = n_filled_rows+1; row <= n_filled_rows+3; row++)
            {
                if (row > N)
                    hash = hash | (mask(w) << msb_index);
                msb_index += w;
            }

            hash = hash + n_filled_rows;

            grid_hash = hash;
        }

    public:
        THash state_hash;
        THash grid_hash;
        ui n_filled_rows;
        State() { }
        State(THash grid_hash, ui n_filled_rows)
        { 
            this->height = min((ui)3, N - n_filled_rows);
            this->width = M+1;
            THash state_hash = grid_hash;
            encode(state_hash, n_filled_rows);
            this->n_filled_rows = n_filled_rows;
            this->state_hash = state_hash;
            this->grid_hash = state_hash >> 5;
        }
        THash get_state_hash() const
        {
            return this->state_hash;
        }
        THash get_grid_hash() const
        {
            return this->grid_hash;
        }
        bool is_stopping_state() const
        {
            return n_filled_rows == N;
        }
        bool try_placing_shape(const Shape shape, THash& grid_hash, ui col, ui row) const
        {
            ui wh = this->width * this->height;
            THash shape_hash = shape.get_hash();
            THash shift_factor = row * this->width + col;
            if (shift_factor + shape.most_significant_one_index > wh)
                return false;
            THash shape_hash_shifted = shape_hash << shift_factor;
            
            if ((shape_hash_shifted & this->grid_hash) == 0)
            {
                grid_hash = shape_hash_shifted | this->grid_hash;
                return true;
            }
            
            return false;
        }
        vector<State> get_next_states(const vector<Shape>& shapes)
        {
            vector<State> next_states;

            // find the first empty position
            ui col = 0;
            ui row = 0;
            bool found = false;
            for (row = 0; row < this->height; row++)
            {
                for (col = 0; col < this->width-1; col++)
                {
                    ui bit = ((this->grid_hash) >> (row * this->width + col)) & mask(1);
                    if (bit == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (found)
                    break;
            }

            if (!found)
                return next_states;


            ui i = 0;
            for (auto shape: shapes)
            {
                i+=1;
                THash next_grid_hash;
                ui next_n_filled_rows = this->n_filled_rows;
                
                // Shape 4
                //   *
                // * *
                // This shape is special because it doesn't occupy position (0,0)
                if (i == 4 && col > 0)
                {
                    if (this->try_placing_shape(shape, next_grid_hash, col-1, row))
                        next_states.push_back(State(next_grid_hash, next_n_filled_rows));
                }
                if (this->try_placing_shape(shape, next_grid_hash, col, row))
                {
                    next_states.push_back(State(next_grid_hash, next_n_filled_rows));
                }
            }

            return next_states;
        }

        static State get_initial_state()
        {
            ui h = 3;
            ui w = M+1;
            THash grid_hash = 0;

            // place a fence at the end of each row
            for (ui row = 0; row < h; row++)
                grid_hash = grid_hash | (((THash)1) << ((w-1) + (w*row)));

            State initial_state = State(grid_hash, 0);

            return initial_state;
        }

        void print_state()
        {
            ui w = M+1;
            ui h = N;

            cout << this->n_filled_rows << " ";

            cout << '[';
            THash hash = this->get_grid_hash();
            for (ui row = this->n_filled_rows; row < this->n_filled_rows+3; row++)
            {
                for (ui col = 0; col < w; col++)
                {
                    if (col == w-1)
                    {
                        if (row == this->n_filled_rows+2)
                            cout << "]";
                        else
                            cout << '|';
                    }
                    else
                    {
                        if ((hash & mask(1)) == 1)
                            cout << "#";
                        else
                            cout << ".";
                    }
                    hash >>= 1;
                }
            }
        }
};

ui dfs(State state,
       const vector<Shape>& shapes,
       unordered_map<THash, ui>& cache)
{

    if (state.is_stopping_state())
        return 1;

    ui ans = 0;

    vector<State> next_states = state.get_next_states(shapes);

    for (auto next_state: next_states)
    {
        if (cache.find(next_state.get_state_hash()) == cache.end())
            cache[next_state.get_state_hash()] = dfs(next_state, shapes, cache);
        ans += cache[next_state.get_state_hash()];
    }

    return ans;

}

vector<Shape> get_shapes()
{
    vector<Shape> shapes;

    Shape shape1;
    shape1.add_tile(0,0);
    shape1.add_tile(1,0);
    shape1.add_tile(0,1);
    shapes.push_back(shape1);

    Shape shape2;
    shape2.add_tile(0,0);
    shape2.add_tile(1,0);
    shape2.add_tile(1,1);
    shapes.push_back(shape2);

    Shape shape3;
    shape3.add_tile(0,0);
    shape3.add_tile(0,1);
    shape3.add_tile(1,1);
    shapes.push_back(shape3);

    Shape shape4;
    shape4.add_tile(1,0);
    shape4.add_tile(0,1);
    shape4.add_tile(1,1);
    shapes.push_back(shape4);

    Shape shape5;
    shape5.add_tile(0,0);
    shape5.add_tile(1,0);
    shape5.add_tile(2,0);
    shapes.push_back(shape5);

    Shape shape6;
    shape6.add_tile(0,0);
    shape6.add_tile(0,1);
    shape6.add_tile(0,2);
    shapes.push_back(shape6);

    return shapes;
}

int main()
{
    ui ans = 0;

    vector<Shape> shapes = get_shapes();

    unordered_map<THash, ui> cache;

    State initial_state = State::get_initial_state();

    ans = dfs(initial_state, shapes, cache); 

    cout << ans << endl;

    return 0;
}
