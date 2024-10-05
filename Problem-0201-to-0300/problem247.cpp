// Problem: https://projecteuler.net/problem=247


#include<iostream>
#include<queue>
#include<vector>
#include<cmath>
#include<utility>
#include<cassert>

using namespace std;

typedef uint64_t ui;

#define endl "\n"


class Square
{
    private:
        double side;
        double x_lowerbound;
        double y_lowerbound;
        ui n_blocks_left;
        ui n_blocks_below;
        // solve ax^2 + bx + c = 0 for real x
        pair<double, double> solve_quadratic_equations(double a, double b, double c)
        {
            double delta = b*b - 4*a*c;
            double sqrt_delta = sqrt(delta);
            pair<double, double> solutions = make_pair((-b + sqrt_delta)/(2*a), (-b - sqrt_delta)/(2*a));
            return solutions;
        }
        double get_side_from_x_y_bounds(double x_lowerbound, double y_lowerbound)
        {
            pair<double, double> solutions = this->solve_quadratic_equations(1.0, x_lowerbound + y_lowerbound, x_lowerbound * y_lowerbound - 1);
            if (solutions.first > 0)
                return solutions.first;
            if (solutions.second > 0)
                return solutions.second;
            //assert(false);
            return -1.0;
        }
    public:
        Square(double x_lowerbound, double y_lowerbound, ui n_blocks_left, ui n_blocks_below)
        {
            this->x_lowerbound = x_lowerbound;
            this->y_lowerbound = y_lowerbound;
            this->n_blocks_left = n_blocks_left;
            this->n_blocks_below = n_blocks_below;
            this->side = this->get_side_from_x_y_bounds(x_lowerbound, y_lowerbound);
        }
        double get_side() const
        {
            return this->side;
        }
        vector<Square> get_next_squares() const
        {
            vector<Square> next_squares;

            // right
            next_squares.push_back(Square(this->x_lowerbound + this->side,
                                          this->y_lowerbound,
                                          this->n_blocks_left + 1,
                                          this->n_blocks_below));
            // up
            next_squares.push_back(Square(this->x_lowerbound,
                                          this->y_lowerbound + this->side,
                                          this->n_blocks_left,
                                          this->n_blocks_below + 1));
            return next_squares;
        }
        ui get_n_blocks_left() const
        {
            return this->n_blocks_left;
        }
        ui get_n_blocks_below() const
        {
            return this->n_blocks_below;
        }
        double get_x_lowerbound() const
        {
            return this->x_lowerbound;
        }
        double get_y_lowerbound() const
        {
            return this->y_lowerbound;
        }
};

bool operator< (const Square& lhs, const Square& rhs)
{
    return lhs.get_side() < rhs.get_side();
}

int main()
{
    ui ans = 0;

    priority_queue<Square> q;

    q.push(Square(1.0, 0.0, 0, 0));

    ui count_smaller_than_3_3 = 1;

    ui n = 0;

    while (count_smaller_than_3_3 > 0)
    {
        n += 1;
        Square square = q.top();
        q.pop();
        if ((square.get_n_blocks_left() <= 3) && (square.get_n_blocks_below() <= 3))
            count_smaller_than_3_3 -= 1;
        vector<Square> next_squares = square.get_next_squares();
        for (auto next_square: next_squares)
        {
            if ((next_square.get_n_blocks_left() <= 3) && (next_square.get_n_blocks_below() <= 3))
                count_smaller_than_3_3 += 1;
            q.push(next_square);
        }
    }

    ans = n;

    cout << ans << endl;

    return 0;
}
