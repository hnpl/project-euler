// https://projecteuler.net/problem=766

#include <iostream>
#include <unordered_set>
#include <array>
#include <utility>
#include <vector>
#include <queue>
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;

typedef uint64_t ui;
typedef __uint128_t Hash;

#define endl "\n"

class Shape
{
    public:
        vector<pair<ui, ui>> occupied_positions;
        Shape() {}
        Shape(const vector<pair<ui, ui>>& occupied_positions) { this->occupied_positions = occupied_positions; }
        static Shape SHAPE_EMPTY;
        static Shape SHAPE_RED;
        static Shape SHAPE_GREEN;
        static Shape SHAPE_YELLOW;
        static Shape SHAPE_BLUE;
        static Shape SHAPE_TEAL;
        static Shape SHAPE_MAGENTA;
};
Shape Shape::SHAPE_EMPTY = Shape();
Shape Shape::SHAPE_RED = Shape({make_pair(0,0), make_pair(1,0), make_pair(0,1)});
Shape Shape::SHAPE_GREEN = Shape({make_pair(0,1), make_pair(1,0), make_pair(1,1)});
Shape Shape::SHAPE_YELLOW = Shape({make_pair(0,0), make_pair(0,1)});
Shape Shape::SHAPE_BLUE = Shape({make_pair(0,0), make_pair(1,0), make_pair(0,1), make_pair(1,1)});
Shape Shape::SHAPE_TEAL = Shape({make_pair(0,0), make_pair(1,0)});
Shape Shape::SHAPE_MAGENTA = Shape({make_pair(0,0)});

class Board
{
    public:
        static ui SHAPE_EMPTY;
        static ui SHAPE_RED;
        static ui SHAPE_GREEN;
        static ui SHAPE_YELLOW;
        static ui SHAPE_BLUE;
        static ui SHAPE_TEAL;
        static ui SHAPE_MAGENTA;
        static array<Shape, 7> SHAPES;

        ui n_cols;
        ui n_rows;
        vector<pair<ui, ui>> shape_positions;
        vector<ui> shape_types;
        Hash hash;

        Board() {}
        Board(ui n_cols, ui n_rows, const vector<pair<ui, ui>>& shape_positions = {}, const vector<ui>& shape_types = {}, Hash hash = 0)
        {
            this->n_cols = n_cols;
            this->n_rows = n_rows;
            this->shape_positions = shape_positions;
            this->shape_types = shape_types;
            this->hash = hash;
        }
        static Board get_init_configuration()
        {
            Board initial_board(6, 5);
            initial_board.try_adding(make_pair(1, 0), Board::SHAPE_RED);
            initial_board.try_adding(make_pair(4, 0), Board::SHAPE_RED);
            initial_board.try_adding(make_pair(2, 0), Board::SHAPE_GREEN);
            initial_board.try_adding(make_pair(4, 3), Board::SHAPE_GREEN);
            initial_board.try_adding(make_pair(5, 1), Board::SHAPE_YELLOW);
            initial_board.try_adding(make_pair(4, 2), Board::SHAPE_YELLOW);
            initial_board.try_adding(make_pair(2, 2), Board::SHAPE_BLUE);
            initial_board.try_adding(make_pair(2, 4), Board::SHAPE_TEAL);
            initial_board.try_adding(make_pair(0, 2), Board::SHAPE_MAGENTA);
            initial_board.try_adding(make_pair(0, 3), Board::SHAPE_MAGENTA);
            initial_board.try_adding(make_pair(0, 4), Board::SHAPE_MAGENTA);
            initial_board.try_adding(make_pair(1, 2), Board::SHAPE_MAGENTA);
            initial_board.try_adding(make_pair(1, 3), Board::SHAPE_MAGENTA);
            initial_board.try_adding(make_pair(1, 4), Board::SHAPE_MAGENTA);
            return initial_board;
        }
        bool try_adding(const pair<ui, ui>& shape_position, const ui& shape_type)
        {
            pair<bool, Hash> result = this->update_hash_for_adding(shape_position, shape_type);
            bool success = result.first;
            Hash new_hash = result.second;
            if (!success)
                return false;
            this->shape_positions.push_back(shape_position);
            this->shape_types.push_back(shape_type);
            this->hash = new_hash;
            return true;
        }
        void remove(const ui& shape_index)
        {
            pair<ui, ui> shape_position = this->shape_positions[shape_index];
            ui shape_type = this->shape_types[shape_index];
            Hash new_hash = this->remove_shape_from_hash(shape_position, shape_type);
            this->hash = new_hash;
            shape_positions.erase(shape_positions.begin() + shape_index);
            shape_types.erase(shape_types.begin() + shape_index);
        }
        pair<bool, Hash> update_hash_for_adding(const pair<ui, ui>& new_shape_position, const ui& new_shape_type)
        {
            ui new_shape_col = new_shape_position.first;
            ui new_shape_row = new_shape_position.second;
            Hash new_hash = this->hash;
            for (auto offset: Board::SHAPES[new_shape_type].occupied_positions)
            {
                ui col_offset = offset.first;
                ui row_offset = offset.second;
                ui new_col = new_shape_col + col_offset;
                ui new_row = new_shape_row + row_offset;
                if ((new_col >= this->n_cols) || (new_row >= this->n_rows))
                    return make_pair(false, 0);
                Hash tile_index = new_row * this->n_cols + new_col;
                Hash curr_tile_hash = (new_hash >> (tile_index * 3)) & (Hash)0x7;
                if (curr_tile_hash != Board::SHAPE_EMPTY)
                    return make_pair(false, 0);
                new_hash |= ((Hash)new_shape_type << (tile_index * 3));
            }
            return make_pair(true, new_hash);
        }
        Hash remove_shape_from_hash(const pair<ui, ui>& shape_position, const ui& shape_type)
        {
            ui shape_col = shape_position.first;
            ui shape_row = shape_position.second;
            Hash new_hash = this->hash;
            for (auto offset: Board::SHAPES[shape_type].occupied_positions)
            {   
                ui col_offset = offset.first;
                ui row_offset = offset.second;
                ui col = shape_col + col_offset;
                ui row = shape_row + row_offset;
                Hash tile_index = row * this->n_cols + col;
                Hash mask = ((Hash)1 << (this->n_rows*this->n_cols*3))-1;
                mask -= ((Hash)(0x7) << (tile_index*3));
                new_hash &= mask;
            }
            return new_hash;
        }
        Hash get_hash()
        {
            return this->hash;
        }
        vector<Board> get_neighbors()
        {
            vector<Board> neighbors;
            
            ui n = this->shape_positions.size();
            for (ui moving_shape_index = 0; moving_shape_index < n; moving_shape_index++)
            {
                Board copied_board = Board(this->n_cols, this->n_rows, this->shape_positions, this->shape_types, this->hash);
                ui moving_shape_col = this->shape_positions[moving_shape_index].first;
                ui moving_shape_row = this->shape_positions[moving_shape_index].second;
                ui moving_shape_type = this->shape_types[moving_shape_index];
                copied_board.remove(moving_shape_index);
                static vector<pair<int, int>> deltas = {make_pair(0,1), make_pair(1,0), make_pair(0,-1), make_pair(-1,0)};
                for (auto delta: deltas)
                {
                    ui delta_col = delta.first;
                    ui delta_row = delta.second;
                    ui new_shape_col = moving_shape_col + delta_col;
                    ui new_shape_row = moving_shape_row + delta_row;
                    if ((new_shape_col == (Hash)-1) || (new_shape_col >= this->n_cols) || (new_shape_row == (Hash)-1) || (new_shape_row >= this->n_rows))
                        continue;
                    bool success = copied_board.try_adding(make_pair(new_shape_col, new_shape_row), moving_shape_type);
                    if (success)
                    {
                        neighbors.push_back(copied_board);
                        copied_board.remove(n-1);
                    }
                }
            }
            return neighbors;
        }
};
ui Board::SHAPE_EMPTY = 0;
ui Board::SHAPE_RED = 1;
ui Board::SHAPE_GREEN = 2;
ui Board::SHAPE_YELLOW = 3;
ui Board::SHAPE_BLUE = 4;
ui Board::SHAPE_TEAL = 5;
ui Board::SHAPE_MAGENTA = 6;
array<Shape, 7> Board::SHAPES = {Shape::SHAPE_EMPTY, Shape::SHAPE_RED, Shape::SHAPE_GREEN, Shape::SHAPE_YELLOW, Shape::SHAPE_BLUE, Shape::SHAPE_TEAL, Shape::SHAPE_MAGENTA};

int main()
{
    unordered_set<Hash> seen_hashes;
    
    auto initial_board = Board::get_init_configuration();
    
    queue<Board> frontiers;
    frontiers.push(initial_board);
    seen_hashes.insert(initial_board.get_hash());

    while (!frontiers.empty())
    {
        auto next_frontier = frontiers.front();
        for (auto neighbor: next_frontier.get_neighbors())
        {
            Hash neighbor_hash = neighbor.get_hash();
            if (seen_hashes.find(neighbor_hash) != seen_hashes.end())
                continue;
            seen_hashes.insert(neighbor_hash);
            frontiers.push(neighbor);
        }
        frontiers.pop();
    }

    ui ans = seen_hashes.size();
    cout << ans << endl;

    return 0;
}
