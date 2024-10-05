// Question: https://projecteuler.net/problem=475

// g++ problem475.cpp -I$HOME/opt/include -L$HOME/opt/lib -lpari -O3

/*
 * . This problem could be thought of as, we have numbers in the range of [0..12n-1],
 * and we want to group the numbers into triplets such that, p and q won't be in the same triplet if p/4 == q/4.
 * . We build the triplets by adding a group of [4k...4k+3] into the triplets group-by-group.
 *   . At each step, we move each number into a different triplet.
 * . Define state as [n_3s, n_2s, n_1s]
 *     where n_3s is the number of triplets with 3 spots left,
 *     similar for n_2s and n_1s.
 * . Suppose at some state, we want to move from state [a_3, a_2, a_1], and suppose we want to add c_3 numbers into n_3s triplets (which will turn into n_2s), c_2 numbers into n_2s triplets, and c_1 number into n_1s triplets.
 *     . The new state is, [a_3 - c_3, a_2 + c_3 - c_2, a_1 + c_2 - c_1].
 *     . The number of ways of doing this is,
 *       C(a_2, c_2) * C(a_1, c_1) * (c_2)! * (c_1)! * C(4, c_3) * C(4-c_3, c_2)
 *       this is because,
 *         . C(a_2, c_2) is the number of ways of choosing c_2 triplets from a_2 existing n_2s triplets,
 *         . C(a_1, c_1) is the number of ways of choosing c_1 triplets from a_1 existing n_1s triplets,
 *         . note that we don't choose c_3 triplets as they are n_3s triplets (empty triplets) and we don't have order for the triplets
 *         . C(4, c_3) is the number of ways of choosing c_3 numbers in the [4k...4k+3] group to put into n_3s triplets,
 *         . C(4-c_3, c_2) is the number of ways of choosing c_2 numbers in the group to put into n_2s triplets, after we did choose c_3 numbers,
 *         . 1 is the number of ways of choosing c_1 numbers in the group to put into n_1s triplets,
 *         . (c_2)! is the number of ways of matching the c_2 numbers (picked from C(4-c_3, c_2)) with the n_2s triplets that we chose (picked from C(a_2, c_2)),
 *         . (c_1)! is the number of ways of matching the c_1 numbers (that will be put into n_1s triplets) with the n_1s triplets that we chose (picked from C(a_1, c_1)),
 *         . note that we don't match the c_3 numbers picked from C(4, c_3). This is because for newly formed triplets, we fix the first number of the triplet to avoid double counting.
 */

#include<pari/pari.h>

#include<iostream>
#include<unordered_map>
#include<algorithm>
#include<vector>
#include<array>
#include<string>
#include<sstream>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = 600;
const ui MOD = pow(10,9)+7;
const ui BIT_WIDTH = 8;

vector<ui> cached_factorials;
vector<ui> cached_inverse_factorials;
unordered_map<ui, unordered_map<ui, ui>> cached_binomial;

constexpr ui get_mask(const ui& n)
{
    return (1ULL << n) - 1;
};

ui inverse_mod(const ui n, const ui mod)
{
    uint64_t t1;
    return Fl_invgen(n, mod, &t1);
}

ui C(const ui& n, const ui& k)
{
    if (cached_binomial.find(n) == cached_binomial.end())
        cached_binomial[n] = unordered_map<ui, ui>();
    if (cached_binomial[n].find(k) == cached_binomial[n].end())
    {
        ui b = 1;
        b *= cached_factorials[n] * cached_inverse_factorials[k];
        b %= MOD;
        b *= cached_inverse_factorials[n-k];
        b %= MOD; 
        cached_binomial[n][k] = b;
    }
    return cached_binomial[n][k];
};

class State
{
    private:
        ui n_3s, n_2s, n_1s, hash;
    public:
        State() {}
        State(const ui& n_3s, const ui& n_2s, const ui& n_1s)
        {
            this->n_3s = n_3s;
            this->n_2s = n_2s;
            this->n_1s = n_1s;
            this->hash = State::encode(n_3s, n_2s, n_1s);
        }
        State(const ui& hash)
        {
            array<ui, 3> decoded_state = State::decode(hash);
            this->n_3s = decoded_state[0];
            this->n_2s = decoded_state[1];
            this->n_1s = decoded_state[2];
            this->hash = hash;
        }
        ui get_hash() const
        {
            return this->hash;
        }
        static ui encode(const ui& n_3s, const ui& n_2s, const ui& n_1s)
        {
            ui h = 0;
            ui msb = 0;
            h |= (n_3s << msb);
            msb += BIT_WIDTH;
            h |= (n_2s << msb);
            msb += BIT_WIDTH;
            h |= (n_1s << msb);
            return h;
        }
        static array<ui, 3> decode(ui hash)
        {
            array<ui, 3> decoded_state;
            decoded_state[0] = hash & get_mask(BIT_WIDTH);
            hash >>= BIT_WIDTH;
            decoded_state[1] = hash & get_mask(BIT_WIDTH);
            hash >>= BIT_WIDTH;
            decoded_state[2] = hash & get_mask(BIT_WIDTH);
            return decoded_state;
        }
        string to_string() const
        {
            stringstream s;
            s << "(" << this->n_3s << ", " << this->n_2s << ", " << this->n_1s << ")";
            return s.str();
        }
        unordered_map<ui, ui> get_next_states()
        {
            unordered_map<ui, ui> next_states;
            for (ui count_3 = 0; count_3 <= 4; count_3++)
            {
                for (ui count_2 = 0; count_2 + count_3 <= 4; count_2++)
                {
                    ui count_1 = 4 - count_3 - count_2;
                    if ((count_3 > n_3s) || (count_2 > n_2s) || (count_1 > n_1s))
                        continue;
                    ui new_n_3s = n_3s - count_3;
                    ui new_n_2s = n_2s + count_3 - count_2;
                    ui new_n_1s = n_1s + count_2 - count_1;
                    State next_state = State(new_n_3s, new_n_2s, new_n_1s);
                    ui next_state_hash = next_state.get_hash();
                    if (next_states.find(next_state_hash) == next_states.end())
                        next_states[next_state_hash] = 0;
                    ui c = C(n_2s, count_2) * C(n_1s, count_1);
                    c %= MOD;
                    c *= cached_factorials[count_2];
                    c %= MOD;
                    c *= cached_factorials[count_1];
                    c %= MOD;
                    c *= C(4, count_3);
                    c %= MOD;
                    c *= C(4-count_3, count_2);
                    c %= MOD;
                    next_states[next_state_hash] += c;
                    next_states[next_state_hash] %= MOD;
                }
            }
            return next_states;
        }
};


void warm_cache(const ui& m)
{
    cached_factorials = vector<ui>(m+1, 0);
    cached_factorials[0] = 1;
    cached_inverse_factorials = vector<ui>(m+1, 0);
    cached_inverse_factorials[0] = 1;
    for (ui i = 1; i <= m; i++)
    {
        cached_factorials[i] = (cached_factorials[i-1] * i) % MOD;
        cached_inverse_factorials[i] = (cached_inverse_factorials[i-1] * inverse_mod(i, MOD)) % MOD;
    }
}

ui solve()
{
    warm_cache(N/3);

    unordered_map<ui, ui> map1;
    unordered_map<ui, ui> map2;
    unordered_map<ui, ui>& curr = map1;
    unordered_map<ui, ui>& prev = map2;
    
    State initial_state = State(N/3, 0, 0);
    ui initial_state_hash = initial_state.get_hash();
    curr[initial_state_hash] = 1;

    for (ui i_group = 0; i_group < N / 4; i_group++)
    {
        swap(curr, prev);
        curr.clear();
        
        for (auto prev_p: prev)
        {
            ui prev_state_hash = prev_p.first;
            ui prev_state_count = prev_p.second;
            State prev_state = State(prev_state_hash);
            
            for (auto next_p: prev_state.get_next_states())
            {
                ui next_state_hash = next_p.first;
                ui next_state_count = next_p.second;
                if (curr.find(next_state_hash) == curr.end())
                    curr[next_state_hash] = 0;
                curr[next_state_hash] += (prev_state_count * next_state_count) % MOD;
                curr[next_state_hash] %= MOD;
            }
        }
        cout << i_group << " " << curr.size() << endl;
    }

    ui ans = 0;
    for (auto p: curr)
    {
        ui curr_state_hash = p.first;
        ui curr_state_count = p.second;
        ans += curr_state_count;
    }
    return ans;
}

int main()
{
    warm_cache(N/3);
    ui ans = solve();
    cout << ans << endl;
    return 0;
}

