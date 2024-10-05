// Question: https://projecteuler.net/problem=244

/*
    . Coordinate: x (left/right), y (up/down)
    . Encode each board configuration as follows,
        * First 2 bits encode the x coordinate of the blank tile.
        * First 2 bits encode the y coordinate of the blank tile.
        * The next 16 bits encode the color of the board 16 tiles.
    . For each state, we generate the next states by swapping the blank tile with its neighbors.
      From the initial state, we use BFS to find the shortest paths to reach the target state.
    . Optimization:
        * From a state, for each of its new states, if we see the next state before, and if the path of the next state is longer than 
          the previous path used to reach that state, we don't push that state to the queue.
*/

#include<iostream>
#include<string>
#include<vector>
#include<cassert>
#include<utility>
#include<queue>
#include<unordered_map>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

const ui N = 4;
const ui MOD = 100000007;

constexpr ui pow2(ui n)
{
    if (n == 0)
        return 1;
    return pow2(n-1) * 2;
};

constexpr ui mask(ui n)
{
    return pow2(n) - 1;
};

class State
{
    private:
        ui hash = 0;
        string path;
        ui path_length;
        const static ui RED = 0;
        const static ui BLANK = 0;
        const static ui BLUE = 1;

    public:
        State()
        {
            this->hash = 0;
            this->path = "";
        }

        State(ui hash, string path)
        {
            this->hash = hash;
            this->path = path;
            this->path_length = path.size();
        }

        State(const vector<vector<ui>>& colors, ui blank_x, ui blank_y, string path)
        {
            this->hash = State::encode(colors, blank_x, blank_y);
            this->path = path;
            this->path_length = path.size();
        }

        ui get_hash()
        {
            return this->hash;
        }

        string get_path()
        {
            return this->path;
        }

        ui get_path_length()
        {
            return this->path.size();
        }

        static State get_initial_state()
        {
            vector<vector<ui>> colors = vector<vector<ui>>(N, vector<ui>(N, 0));
            ui blank_x = 0;
            ui blank_y = 0;
            for (ui x = 0; x < N/2; x++)
                for (ui y = 0; y < N; y++)
                    colors[x][y] = RED;
            for (ui x = N/2; x < N; x++)
                for (ui y = 0; y < N; y++)
                    colors[x][y] = BLUE;
            colors[blank_x][blank_y] = RED;
            ui hash = State::encode(colors, blank_x, blank_y);
            return State(hash, "");
        }

        static State get_target_state()
        {
            vector<vector<ui>> colors = vector<vector<ui>>(N, vector<ui>(N, 0));
            ui blank_x = 0;
            ui blank_y = 0;
            for (ui x = 0; x < N; x++)
                for (ui y = 0; y < N; y++)
                {
                    /*
                    if ((x + y) % 2 == 0)
                        colors[x][y] = RED;
                    else
                        colors[x][y] = BLUE;
                    */
                    colors[x][y] = (x+y)%2;
                }
            colors[blank_x][blank_y] = RED;
            ui hash = State::encode(colors, blank_x, blank_y);
            return State(hash, "");
        }

        static ui encode(const vector<vector<ui>>& colors, ui blank_x, ui blank_y)
        {
            ui hash = 0;
            ui msb_index = 0;

            hash = blank_x;
            hash = (blank_y << 2) | hash;

            //assert(colors[blank_x][blank_y] == RED);

            msb_index = 4;
            for (ui x = 0; x < colors.size(); x++)
            {
                for (ui y = 0; y < colors[0].size(); y++)
                {
                    hash = (colors[x][y] << msb_index) | hash;
                    msb_index += 1;
                }
            }

            return hash;
        }

        vector<State> get_next_states()
        {
            vector<vector<ui>> colors = vector<vector<ui>>(N, vector<ui>(N, 0));
            ui blank_x = 0;
            ui blank_y = 0;

            // decoding the current state
            ui _hash = this->hash;

            blank_x = _hash & mask(2);
            _hash >>= 2;

            blank_y = _hash & mask(2);
            _hash >>= 2;

            for (ui x = 0; x < N; x++)
                for (ui y = 0; y < N; y++)
                {
                    colors[x][y] = _hash & mask(1);
                    _hash >>= 1;
                }
            
            // generate the next states by swapping the blank with its neighbors
            vector<State> next_states;

            // L: swapping the blank with the tile on the right
            if (blank_x < (N-1))
            {
                swap(colors[blank_x][blank_y], colors[blank_x+1][blank_y]);
                blank_x++;
                State next_state = State(colors, blank_x, blank_y, this->path + "L");
                next_states.push_back(next_state);
                blank_x--;
                swap(colors[blank_x][blank_y], colors[blank_x+1][blank_y]);
            }

            // R: swapping the blank with the tile on the left
            if (blank_x > 0)
            {
                swap(colors[blank_x][blank_y], colors[blank_x-1][blank_y]);
                blank_x--;
                State next_state = State(colors, blank_x, blank_y, this->path + "R");
                next_states.push_back(next_state);
                blank_x++;
                swap(colors[blank_x][blank_y], colors[blank_x-1][blank_y]);
            }

            // D: swapping the blank tile with the tile beneath it
            if (blank_y > 0)
            {
                swap(colors[blank_x][blank_y], colors[blank_x][blank_y-1]);
                blank_y--;
                State next_state = State(colors, blank_x, blank_y, this->path + "D");
                next_states.push_back(next_state);
                blank_y++;
                swap(colors[blank_x][blank_y], colors[blank_x][blank_y-1]);
            }

            // U: swapping the blank tile with the tile above it
            if (blank_y < (N-1))
            {
                swap(colors[blank_x][blank_y], colors[blank_x][blank_y+1]);
                blank_y++;
                State next_state = State(colors, blank_x, blank_y, this->path + "U");
                next_states.push_back(next_state);
                blank_y--;
                swap(colors[blank_x][blank_y], colors[blank_x][blank_y+1]);
            }

            return next_states;
        }

        ui get_path_checksum()
        {
            ui checksum = 0;
            for (char& move: this->path)
            {
                ui m = 0;
                switch (move)
                {
                    case 'L': m = 76; break;
                    case 'R': m = 82; break;
                    case 'U': m = 85; break;
                    case 'D': m = 68; break;
                }
                checksum = (checksum * 243 + m) % MOD;
            }
            return checksum;
        }

        void print()
        {
            vector<vector<ui>> colors = vector<vector<ui>>(N, vector<ui>(N, 0));
            ui blank_x = 0;
            ui blank_y = 0;

            // decoding the current state
            ui _hash = this->hash;

            blank_x = _hash & mask(2);
            _hash >>= 2;

            blank_y = _hash & mask(2);
            _hash >>= 2;

            for (ui x = 0; x < N; x++)
                for (ui y = 0; y < N; y++)
                {
                    colors[x][y] = _hash & mask(1);
                    _hash >>= 1;
                }

            for (ui y = 0; y < N; y++)
            {
                for (ui x = 0; x < N; x++)
                {
                    if (colors[x][y] == RED)
                    {
                        if ((x == blank_x) && (y == blank_y))
                            cout << " ";
                        else
                            cout << "x";
                    }
                    else
                    {
                        cout << "o";
                    }
                }
                cout << endl;
            }
        }
};

int main()
{
    ui ans = 0;

    ui optimal_n_moves = -1ULL;

    State initial_state = State::get_initial_state();
    State target_state = State::get_target_state();
    ui target_state_hash = target_state.get_hash();
    queue<State> bfs;
    bfs.push(initial_state);

    unordered_map<ui, ui> seen; // seen[state_hash] = n means the minimum number of steps to reach state_hash is n

    while (!bfs.empty())
    {
        State curr_state = bfs.front();
        bfs.pop();

        vector<State> next_states = curr_state.get_next_states();
        for (auto next_state: next_states)
        {
            ui next_state_hash = next_state.get_hash();
            if (seen.find(next_state_hash) != seen.end())
            {
                if (next_state.get_path_length() > seen[next_state_hash])
                    continue;
            }
            else
            {
                seen[next_state_hash] = next_state.get_path_length();
            }

            if (next_state_hash == target_state_hash)
            {
                optimal_n_moves = next_state.get_path_length();
                ans += next_state.get_path_checksum();
                //cout << next_state.get_path() << " " << next_state.get_path_checksum() << endl;
            }
            else if (next_state.get_path_length() < optimal_n_moves)
            {
                bfs.push(next_state);
            }
        }
    }
    cout << ans << endl;

    return 0;
}