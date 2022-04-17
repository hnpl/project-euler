// Question: https://projecteuler.net/problem=393

/*
 * Strategy: dynamic programming with states.
 * - State = a row
 *   + Encoded by, for each cell, whether an arrow coming in/out on the top/bottom.
 */

#include<iostream>
#include<vector>
#include<array>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include<sstream>
#include<string>
#include<algorithm>
#include<cassert>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

const ui N = 10;

constexpr ui pow2(ui n)
{
    return 1ULL << n;
};


const ui NO_ARROW  = 0b00; // no arrow comming in or out
const ui ARROW_IN  = 0b01; // must have an arrow comming in
const ui ARROW_OUT = 0b10; // has an arrow out

const ui SHAPE_1 = 0;  // arrow down
const ui SHAPE_2 = 1;  // arrow up
const ui SHAPE_3 = 2;  // arrow right
const ui SHAPE_4 = 3;  // arrow left
const ui SHAPE_5 = 4;  // bottom -> center -> right
const ui SHAPE_6 = 5;  // bottom -> center -> left
const ui SHAPE_7 = 6;  // right  -> center -> top
const ui SHAPE_8 = 7;  // right  -> center -> bottom
const ui SHAPE_9 = 8;  // top    -> center -> right
const ui SHAPE_A = 9;  // top    -> center -> left
const ui SHAPE_B = 10; // left   -> center -> top
const ui SHAPE_C = 11; // left   -> center -> bottom
const array<ui, 12> SHAPES = {SHAPE_1, SHAPE_2, SHAPE_3, SHAPE_4,
                              SHAPE_5, SHAPE_6, SHAPE_7, SHAPE_8,
                              SHAPE_9, SHAPE_A, SHAPE_B, SHAPE_C};
//                                         1  2  3  4  5  6  7  8  9  A  B  C                                    
const array<bool, 12> IN_FROM_TOP =       {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0};
const array<bool, 12> IN_FROM_BOTTOM =    {0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
const array<bool, 12> OUT_FROM_TOP =      {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0};
const array<bool, 12> OUT_FROM_BOTTOM =   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1};
const array<bool, 12> CAN_BE_RIGHT_MOST = {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1};
const vector<ui> CAN_BE_LEFT_MOST = {
    SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9
};

// which shapes should be on the right of a shape
const vector<vector<ui>> LEFT_RIGHT_MATCHING = {
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}, // SHAPE_1
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}, // SHAPE_2
    {SHAPE_3, SHAPE_B, SHAPE_C},                            // SHAPE_3
    {SHAPE_4, SHAPE_6, SHAPE_A},                            // SHAPE_4
    {SHAPE_3, SHAPE_B, SHAPE_C},                            // SHAPE_5
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}, // SHAPE_6
    {SHAPE_4, SHAPE_6, SHAPE_A},                            // SHAPE_7
    {SHAPE_4, SHAPE_6, SHAPE_A},                            // SHAPE_8
    {SHAPE_3, SHAPE_B, SHAPE_C},                            // SHAPE_9
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}, // SHAPE_A
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}, // SHAPE_B
    {SHAPE_1, SHAPE_2, SHAPE_5, SHAPE_7, SHAPE_8, SHAPE_9}  // SHAPE_C
};

class State
{
    public:
        ui n;
        ui hash;

        State()
        {
            this->n = 0;
            this->hash = 0;
        }

        State(ui n)
        {
            this->n = n;
            this->hash = 0;
        }

        State(ui n, ui hash)
        {
            this->n = n;
            this->hash = hash;
        }
        
        State(ui n, const array<ui, N>& form)
        {
            this->n = n;
            this->calculate_hash(n, form);
        }

        void calculate_hash(const ui& n, const array<ui, N>& form)
        {
            this->hash = 0;
            for (ui pos = 0; pos < n; pos++)
            {
                ui shape = form[pos];
                this->set_top(pos, ((OUT_FROM_TOP[shape]) << 1) | IN_FROM_TOP[shape]);
                this->set_bottom(pos, ((OUT_FROM_BOTTOM[shape] << 1) | IN_FROM_BOTTOM[shape]));
            }
        }

        bool is_top_row() const
        {
            return this->top() == 0;
        }

        bool is_bottom_row() const
        {
            return this->bottom() == 0;
        }

        void set_top(ui pos, ui status) // 0b00: no arrow; 0b10: arrow must in; 0b11: has an arrow out
        {
            //this->hash ^= 3ULL << (2*pos);
            this->hash |= (status & 0x3) << (2*pos);
        }

        void set_bottom(ui pos, ui status)
        {
            //this->hash ^= 3ULL << (2*pos + 2*n);
            this->hash |= (status & 0x3) << (2*pos + 2*n);
        }

        ui get_top_status(ui pos) const
        {
            return (this->hash >> (2*pos)) & 0x3;
        }

        ui get_bottom_status(ui pos) const
        {
            return (this->hash >> (2*pos + 2*n)) & 0x3;
        }

        //bool is_compatible(const State& other) const // stack the other row on top of this row
        //{
        //    bool ans = true;
        //    for (ui i = 0; i < n; i++)
        //    {
        //        ans &= ((this->get_top_status(i) + other.get_bottom_status(i)) == 0) || ((this->get_top_status(i) + other.get_bottom_status(i)) == 3);
        //    }
        //    if (ans)
        //    {
        //        for (ui i = 0; i < n; i++)
        //        {
        //            assert(   ((this->get_top_status(i) == 0) && (other.get_bottom_status(i) == 0))
        //                   || ((this->get_top_status(i) == 1) && (other.get_bottom_status(i) == 2))
        //                   || ((this->get_top_status(i) == 2) && (other.get_bottom_status(i) == 1))
        //                  );
        //        }
        //   }
        //    return ans;
        //}

        ui get_compatible_lower_row_top()
        {
            State lower(n);
            for (ui i = 0; i < n; i++)
                if (this->get_bottom_status(i) == 0)
                    lower.set_top(i, 0);
                else
                    lower.set_top(i, 3 - this->get_bottom_status(i));
            return lower.top();
        }

        //static bool is_compatible(const ui& n, const ui& upper_hash, const ui& lower_hash)
        //{
        //    State upper = State(n, upper_hash);
        //    State lower = State(n, lower_hash);
            //cout << "Compatible test:" << endl;
            //cout << "Upper: " << State(N, upper_hash).to_str() << endl;
            //cout << "Lower: " << State(N, lower_hash).to_str() << endl;
            //cout << "test" << lower.is_compatible(upper) << endl;
        //    return lower.is_compatible(upper);
        //}

        static bool is_bottom_row_hash(const ui& n, const ui& bottom_hash)
        {
            return bottom_hash == 0;
        }

        ui top() const
        {
            return this->hash & (pow2(2*n) - 1);
        }

        ui bottom() const
        {
            return (this->hash >> (2*n)) & (pow2(2*n) - 1);
        }

        string to_str()
        {
            stringstream s;
            s << "top: [";
            for (ui i = 0; i < n; i++)
                s << get_top_status(i) <<",";
            s << "] bottom: [";
            for (ui i = 0; i < n; i++)
                s << get_bottom_status(i) << ",";
            s <<"]";
            s << is_top_row() << " " << is_bottom_row();
            return s.str();
        }
};

void discover_rows(ui depth, const ui& max_depth, array<ui, N>& path, unordered_map<ui, unordered_map<ui, ui>>& row_count, unordered_map<ui, ui>& compatible_bottom_top)
{
    if (depth == max_depth)
    {
        if (!CAN_BE_RIGHT_MOST[path[max_depth-1]])
            return;
        State new_row_state = State(max_depth, path);
        ui new_state_top = new_row_state.top();
        ui new_state_bottom = new_row_state.bottom();
        if (row_count.find(new_state_top) == row_count.end())
            row_count[new_state_top] = unordered_map<ui, ui>();
        if (row_count[new_state_top].find(new_state_bottom) == row_count[new_state_top].end())
            row_count[new_state_top][new_state_bottom] = 0;
        row_count[new_state_top][new_state_bottom] += 1;
        if (compatible_bottom_top.find(new_state_bottom) == compatible_bottom_top.end())
            compatible_bottom_top[new_state_bottom] = new_row_state.get_compatible_lower_row_top();
        //if (new_row_state.is_top_row())
        //    top_rows.insert(new_state_hash);
        //if (new_row_state.is_bottom_row())
        //    bottom_rows.insert(new_state_hash);
        return;
    }

    if (depth == 0)
    {
        for (auto shape: CAN_BE_LEFT_MOST)
        {
            path[depth] = shape;
            discover_rows(depth+1, max_depth, path, row_count, compatible_bottom_top);
        }
    }
    else
    {
        for (auto shape: LEFT_RIGHT_MATCHING[path[depth-1]])
        {
            path[depth] = shape;
            discover_rows(depth+1, max_depth, path, row_count, compatible_bottom_top);
        }
    }
}

int main()
{
    ui ans = 0;

    array<ui, N> path;
    unordered_map<ui, unordered_map<ui, ui>> row_count;
    unordered_map<ui, ui> compatible_bottom_top;
    //unordered_set<ui> top_rows;
    //unordered_set<ui> bottom_rows;
    //discover_rows(0, N, path, row_count, top_rows, bottom_rows);
    discover_rows(0, N, path, row_count, compatible_bottom_top);
    cout << row_count.size() << endl;
    //cout << top_rows.size() << endl;
    //cout << bottom_rows.size() << endl;

    unordered_map<ui, ui> map1;
    unordered_map<ui, ui> map2;

    unordered_map<ui, ui>& curr_top_hashes = map1;
    unordered_map<ui, ui>& next_top_hashes = map2;

    next_top_hashes[0] = 1;

    for (ui i = 1; i < N; i++)
    {
        swap(curr_top_hashes, next_top_hashes);
        next_top_hashes.clear();

        for (auto p: curr_top_hashes)
        {
            ui curr_top_hash = p.first;
            ui curr_top_count = p.second;

            for (auto p: row_count[curr_top_hash])
            {
                ui curr_bottom_hash = p.first;
                ui curr_hash_count = p.second * curr_top_count;
                ui next_top_hash = compatible_bottom_top[curr_bottom_hash];
                if (next_top_hashes.find(next_top_hash) == next_top_hashes.end())
                   next_top_hashes[next_top_hash] = 0;
               next_top_hashes[next_top_hash] += curr_hash_count; 
            }
        }
    }

    // do the N^th iteration outside the loop to calculate the answer
    swap(curr_top_hashes, next_top_hashes);
    for (auto p: curr_top_hashes)
    {
        ui curr_top_hash = p.first;
        ui curr_top_count = p.second;

        for (auto p: row_count[curr_top_hash])
        {
            ui curr_bottom_hash = p.first;
            ui curr_hash_count = p.second * curr_top_count;
            if (State::is_bottom_row_hash(N, curr_bottom_hash))
            {
                ans += curr_hash_count;
            }
        }
    } 

    cout << ans << endl;

    return 0;
}
