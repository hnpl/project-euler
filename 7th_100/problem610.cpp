// Problem: https://projecteuler.net/problem=610

/*
    - Since we can have an arbitrary number of M's, the state space will grow exponentially.
    - So, first, we calculate the expectation of the value without leading M, call this base_expectation
    - Then, we iterate through the combinations with K leading M's and calculate the expectation of Roman numbers with less than or equal K leading M's
        . new_expectation = 0.14^K * 0.86 * (1000*K+base_expectation)
        . do this for all K from 0 to n until the expectation converges
*/

#include<iostream>
#include<string>
#include<regex>
#include<vector>
#include<array>
#include<unordered_map>
#include<iomanip>
#include<cassert>

using namespace std;

#define endl "\n"

const vector<char> alphabet = {'I', 'V', 'X', 'L', 'C', 'D', 'M', '#'};
const vector<double> alphabet_probability = {0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.14, 0.02};
const int alphabet_size = alphabet.size();

const vector<string> evaluation_alphabet = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I", "#"};
const vector<int> evaluation_alphabet_value = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1, 0};
const vector<int> evaluation_alphabet_n_chars_to_remove = {1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1};
const int evaluation_alphabet_size = evaluation_alphabet.size();

int evaluate_roman_numeral(string roman_numeral)
{
    static regex pattern = regex("^(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})[#]?$"); // without leading M
    int val = 0;
    int string_len = roman_numeral.length();
    if (regex_match(roman_numeral, pattern))
    {
        int idx = 0;
        int prev_i = 0;
        int n_chars_to_remove = 0;
        while (idx < string_len)
        {
            n_chars_to_remove = 1;
            for (int i = 0; i < evaluation_alphabet_size; i++)
            {
                if (roman_numeral.find(evaluation_alphabet[i], idx) == idx)
                {
                    val = val + evaluation_alphabet_value[i];
                    idx += evaluation_alphabet_n_chars_to_remove[i];
                    break;
                }
            }
        }
    }
    else
        return -1;
    return val;
}



class State
{
    public:
        string roman_numeral;
        double probability;
        State(const string& roman_numeral, double probability)
        {
            this->roman_numeral = roman_numeral;
            this->probability = probability;
        }
        
        bool is_stopping_state()
        {
            if (this->roman_numeral.length() == 0)
                return false;
            return this->roman_numeral[this->roman_numeral.length()-1] == '#';
        }

        vector<State> get_next_states()
        {
            vector<State> next_states;

            for (int i = 0; i < alphabet_size; i++)
                    next_states.push_back(move(State(this->roman_numeral + alphabet[i],
                                                     this->probability * alphabet_probability[i])));

            return next_states;
        }

        bool is_generating_state()
        {
            vector<State> next_states = this->get_next_states();
            for (auto state: next_states)
            {
                if (evaluate_roman_numeral(state.roman_numeral) >= 0)
                    return true;
            }
            return false;
        }
};

int main()
{
    double ans = 0.0;

    cout << setprecision(15);

    array<unordered_map<string, double>, 2> states;

    unordered_map<string, double> all_states;

    int curr_row = 0;
    int prev_row = 1 - curr_row;

    states[curr_row][""] = 1.0 - 0.14; // all combinations except leading M

    double tol = 1e-12;

    double prev_expectation = 0.0;
    double curr_expectation = 0.0;

    int iter = 0;

    while ((curr_expectation - prev_expectation > tol) || (curr_expectation < tol))
    {
        prev_expectation = curr_expectation;
        curr_row = 1 - curr_row;
        prev_row = 1 - prev_row;

        states[curr_row].clear();

        unordered_map<string, double>& curr_states = states[curr_row];
        for (auto p: states[prev_row])
        {
            string prev_state_roman_numeral = p.first;
            double prev_state_probability = p.second;
            State prev_state = State(prev_state_roman_numeral, prev_state_probability);

            if (prev_state.is_generating_state())
            {
                vector<State> next_states = prev_state.get_next_states();
                for (auto next_state: next_states)
                {
                    string next_state_roman_numeral = next_state.roman_numeral;
                    double next_state_probability = next_state.probability;
                    int next_state_value = evaluate_roman_numeral(next_state_roman_numeral);
                    if (next_state.is_stopping_state())
                    {
                        if (next_state_value > 0)
                            curr_expectation += next_state_value * next_state_probability;
                        if (all_states.find(next_state_roman_numeral) == all_states.end())
                            all_states[next_state_roman_numeral] = next_state_probability;
                        else
                            all_states[next_state_roman_numeral] += next_state_probability;
                        continue;
                    }
                    if (next_state_value < 0)
                        next_state_roman_numeral = next_state_roman_numeral.substr(0, next_state_roman_numeral.length()-1);
                    if (curr_states.find(next_state_roman_numeral) == curr_states.end())
                        curr_states[next_state_roman_numeral] = next_state_probability;
                    else
                        curr_states[next_state_roman_numeral] += next_state_probability;
                }
            }
        }
    }


    double M_coeff = 1.0;
    prev_expectation = 0;
    int n_M = 0;
    double base_expectation = curr_expectation / (1-0.14); // rescale expectation as we multiplied the current expectation with 0.86 to ignore the cases with leading M
    while ((curr_expectation - prev_expectation > tol))
    {
        n_M += 1;
        prev_expectation = curr_expectation;
        M_coeff = M_coeff * 0.14; // the first n_M letters are M's
        double M_coeff_2 = M_coeff * 0.86; // the (n_M+1)th letter shouldn't be M
        /*
        for (auto state: all_states)
        {
            string state_roman_numeral = state.first;
            double state_probability = state.second;

            double next_state_probability = M_coeff_2 * state_probability / (1-0.14);
            int next_state_roman_numeral_value = 1000 * n_M + evaluate_roman_numeral(state_roman_numeral);
            curr_expectation += next_state_roman_numeral_value * next_state_probability;
        }*/
        curr_expectation += M_coeff_2 * (1000 * n_M + base_expectation);
    }

    ans = curr_expectation;

    cout << ans << endl;

    return 0;
}