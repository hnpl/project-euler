// Question: https://projecteuler.net/problem=869

/*
 * . Ideas,
 *   - Decode the primes to their binary forms.
 *   - Build a suffix tree based on the binary representation, where the root is where we make the decision of choosing 0 or 1 for the least significant bit.
 *     In this implementation, the left child node represents choosing 0 and the right child node represents choosing 1.
 *   - Note that a prime number can be an internal node of the tree.
 *   - Each node of the tree represents decision making after a suffix (some least significant bits) is determined.
 *     We choose 0 or 1 based on which child node has more prime descendents.
 *     We can count how many prime descendents each node has using DFS.
 *   - Note that, after counting how many prime descendents each node has, we know exactly which number (0 or 1) we should guess at that point.
 *     Therefore, by constructing a path (using DFS) from the root to each prime number node, we know the exact score for each prime number.
 *   - Also, each prime number appears at the probability of 1/N where N is the number of prime numbers less than 10**8.
 *     Note that E(P_1+P_2+...+P_N) = E(P_1) + E(P_2) + ... + E(P_N)
 *                                  = Prob(P_1) * score(P_1) + Prob(P_2) * score(P_2) + ... + Prob(P_N) * score(P_N)
 *                                  = 1/N * (score(P_1) + score(P_2) + ... + score(P_N))
 *     We can obtain the score for each prime number P_i using the idea above.
 */

#include<algorithm>
#include<array>
#include<cassert>
#include<iostream>
#include<iomanip>
#include<math.h>
#include<queue>
#include<stdint.h>
#include<vector>
#include<unordered_map>
#include<utility>

using namespace std;

typedef uint64_t ui;
#define endl "\n"

constexpr ui pow(const ui& base, const ui& exp)
{
    if (exp == 0)
        return 1;
    return base * pow(base, exp-1);
};

const ui N = pow(10, 8);
ui NUM_BITS = int(ceil(log2(N)))-1;
ui NUM_BITS_BITS = int(ceil(log2(NUM_BITS)));

vector<ui> get_primes(ui n)
{
    vector<ui> primes;
    primes.reserve(n/10);

    vector<bool> is_prime = vector<bool>(n/2+1, true);
    ui sqrt_n = ui(sqrt(n));
    for (ui p = 3; p <= sqrt_n; p+=2)
    {
        if (!is_prime[(p-3)/2])
            continue;
        for (ui np = 3*p; np <= n; np += 2*p)
            is_prime[(np-3)/2] = false;
    }
    primes.push_back(2);
    ui size = is_prime.size();
    for (ui p = 0; 2*p+3 <= n; p++)
    {
        if (is_prime[p])
            primes.push_back(2*p+3);
    }
    return primes;
}

constexpr ui mask(const ui& bit_idx)
{
    return 1ULL << bit_idx;
};

class Node
{
    public:
        Node* parent;
        Node* left_child;
        Node* right_child;
        ui val;
        ui num_bits;
        bool is_prime_node;
        ui num_prime_descendents;
        bool guess_1;
        static inline ui hash(const ui& _val, const ui& _num_bits)
        {
            return (_val << NUM_BITS_BITS) | _num_bits;
        }
    public:
        Node()
            : parent(nullptr), left_child(nullptr), right_child(nullptr), val(0), num_bits(0), is_prime_node(false),
              num_prime_descendents(0), guess_1(false)
        {}
        Node(const ui& _val, const ui& _num_bits, const bool& _is_prime)
            : left_child(nullptr), right_child(nullptr), val(_val), num_bits(_num_bits), is_prime_node(_is_prime),
              num_prime_descendents(0), guess_1(false)
        {
            if (is_prime_node)
               num_prime_descendents = 1; 
        }
        ui get_parent_node_hash() const
        {
            ui parent_val = val & (~mask(num_bits-1));
            ui parent_num_bits = num_bits - 1;
            return Node::hash(parent_val, parent_num_bits);
        }
        Node* spawn_parent_node() const
        {
            assert(num_bits > 0);
            return new Node(val & (~mask(num_bits-1)), num_bits - 1, false);
        }
        void print_node() const
        {
            cout << "Node: (" << val << ", " << num_bits << "); prime_descendents: " << num_prime_descendents << "; guess: ";
            if (guess_1)
                cout << "1" << endl;
            else
                cout << "0" << endl;
            if (left_child)
                cout << "-> Left child: (" << left_child->val << ", " << left_child->num_bits <<")" << endl;
            if (right_child)
                cout << "-> Right child: (" <<right_child->val << ", " << right_child->num_bits << ")" << endl;
            cout << "---" << endl;
            if (left_child != nullptr)
                left_child->print_node();
            if (right_child != nullptr)
                right_child->print_node();
        }
        ui count_prime_descendents_and_guess()
        {
            ui num_left_child_children = 0;
            ui num_right_child_children = 0;
            if (left_child)
            {
                num_left_child_children = left_child->count_prime_descendents_and_guess();
                this->num_prime_descendents += left_child->num_prime_descendents;
            }
            if (right_child)
            {
                num_right_child_children = right_child->count_prime_descendents_and_guess();
                this->num_prime_descendents += right_child->num_prime_descendents;
            }
            if (num_left_child_children >= num_right_child_children)
                set_guess_0();
            else
                set_guess_1();
            return this->num_prime_descendents;
        }
        ui aggerate_scores(ui curr_score_sum) // calculate all possible scores contributed by each prime node
        {
            ui score_sum = 0;
            if (left_child) // guessing 0
            {
                if (!guess_1)
                {
                    score_sum += left_child->aggerate_scores(curr_score_sum + 1);
                }
                else
                {
                    score_sum += left_child->aggerate_scores(curr_score_sum);
                }
            }
            if (right_child) // guessing 1
            {
                if (!guess_1)
                {
                    score_sum += right_child->aggerate_scores(curr_score_sum);
                }
                else
                {
                    score_sum += right_child->aggerate_scores(curr_score_sum + 1);
                }
            }
            if (is_prime_node)
            {
                score_sum += curr_score_sum;
            }
            return score_sum;
        }
        ui get_hash() const { return Node::hash(this->val, this->num_bits); }
        inline static pair<ui, ui> decode_hash(const ui& hash)
        {
            return make_pair(hash >> NUM_BITS_BITS, hash & ((1ULL << NUM_BITS_BITS) - 1));
        }
        ui get_num_prime_descendents() const { return num_prime_descendents; }
        void set_parent(Node* v) { parent = v; }
        void set_child(Node* child)
        {
            assert(child->num_bits - 1 == this->num_bits);
            bool child_MSB_is_1 = child->val & (mask(child->num_bits-1));
            if (child->num_bits == 1)
                child_MSB_is_1 = (child->val == 1);
            if (child_MSB_is_1)
                set_right_child(child);
            else
                set_left_child(child);
        }
        void set_left_child(Node* v)
        {
            assert(left_child == nullptr);
            left_child = v;
        }
        void set_right_child(Node* v)
        {
            assert(right_child == nullptr);
            right_child = v;
        }
        bool is_root() const { return parent == nullptr; }
        bool is_leaf() const { return (left_child == nullptr) && (right_child == nullptr); }
        bool is_prime() const { return is_prime_node; }
        void set_as_prime_node()
        {
            assert(!is_prime_node);
            this->num_prime_descendents = 1; // the node self
            is_prime_node = true;
        }
        inline void set_guess_0() { guess_1 = false; }
        inline void set_guess_1() { guess_1 = true; }
};

class Graph
{
    public:
        Node* root;
        vector<Node*> leaves;
        unordered_map<ui, Node*> hash_node_map;
        Graph() {}
        Graph(const ui& num_leaves) { leaves.reserve(num_leaves); }
        ~Graph()
        {
            for (auto p: hash_node_map)
                delete(p.second);
        }
        ui size() const
        {
            return hash_node_map.size();
        }
        Node* add_prime_node(Node* prime_node)
        {
            ui prime_node_hash = prime_node->get_hash();
            if (hash_node_map.find(prime_node_hash) != hash_node_map.end())
            {
                delete prime_node; // This is the case where the prime node is an internal node
                Node* actual_prime_node = hash_node_map[prime_node_hash];
                actual_prime_node->set_as_prime_node();
                //leaves.push_back(actual_prime_node);
                return actual_prime_node;
            }
            else
            {
                Node* w = prime_node;
                while (w->num_bits != 0)
                {
                    ui parent_hash = w->get_parent_node_hash();
                    if (hash_node_map.find(parent_hash) == hash_node_map.end())
                    {
                        Node* parent_node = w->spawn_parent_node();
                        parent_node->set_child(w);
                        w->set_parent(parent_node);
                        assert(parent_node->get_hash() == parent_hash);
                        hash_node_map[parent_hash] = move(parent_node);
                        w = parent_node;
                    }
                    else
                    {
                        auto p = Node::decode_hash(parent_hash);
                        Node* parent_node = hash_node_map[parent_hash];
                        w->set_parent(parent_node);
                        parent_node->set_child(w);
                        break;
                    }
                }
                if (w->num_bits == 0)
                    root = w;
                leaves.push_back(move(prime_node));
                hash_node_map[prime_node_hash] = prime_node;
            }
            return prime_node;
        }
        void count_descendents_and_guess() // pass 1: bottom-up: counting descendents via DFS, then make a guess
        {
            root->count_prime_descendents_and_guess();
        }
        ui get_total_scores()
        {
            return root->aggerate_scores(0);
        }
};

double solve(const ui& n)
{
    double ans = 0.0;
    vector<ui> primes = get_primes(n); 
    Graph g = Graph(primes.size());
    ui num_bits = 1;
    ui max_v = 1ULL;
    for (auto const prime: primes)
    {
        while (max_v < prime)
        {
            num_bits += 1;
            max_v = (1ULL << num_bits) - 1;
        }
        Node* prime_node = new Node(prime, num_bits, true);
        g.add_prime_node(prime_node);
    }
    g.count_descendents_and_guess();
    ui total_score = g.get_total_scores();
    ans = 1.0 * total_score / primes.size();
    return ans;
}

int main()
{
    auto ans = solve(N);
    cout << setprecision(12) <<  ans << endl;
    return 0;
}
