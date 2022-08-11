// https://projecteuler.net/problem=733

/*
 * . Let f(l, n) be the sum of all sequences of length l ending with n.
 * . Let g(l, n) be the number of sequences of length l ending with n.
 * . For all m such that m < n and m appears before n in the sequence,
 *   f(l, n) = sum[f(l-1, m) + g(l-1, m) * n] // this is because we append n to all sequences ending with m for all m.
 * . Without optimizations, the sum takes O(n) per sequence element, thus the total time complexity is O(n^2), which would take ~3 hours to run.
 * . Optimization:
 *    . Let h(l, n) be the sum of all sequences of length l ending with m, for all m < n where m appears before n in the sequence. I.e., h(l, n) is the prefix sum.
 *    . Similarly, let c(l, n) be the number of sequences of length l ending with m, for all m < n where m appears before n in the sequence. I.e., c(l, n) is the prefix count.
 * . We can use Fenwick tree to maintain the prefix sum and the prefix count.
 * . Illustration of using Fenwick tree,
 *   . https://en.wikipedia.org/wiki/Fenwick_tree
 *   . https://i.imgur.com/SH4tJPN.png (from https://vnoi.info/wiki/algo/data-structures/fenwick.md)
 */

#include<algorithm>
#include<vector>
#include<iostream>

using namespace std;

typedef uint64_t ui;

const ui N = 153;
const ui M = 1000000;
//const ui M = 100;
//const ui M = 6;
const ui K = 4;
const ui SEQ_MOD = 10000019;
const ui ANS_MOD = 1000000007;

class SequenceGenerator
{
    private:
        ui next;
    public:
        SequenceGenerator() { this->next = N; }
        ui get_next()
        {
            ui ans = this->next;
            this->next = (this->next * N) % SEQ_MOD;
            return ans;
        }
        void reset()
        {
            this->next = N;
        }
};

class FenwickTreePrefixSum
{
    private:
        vector<ui> tree;
        ui size;
    public:
        FenwickTreePrefixSum(const ui& size)
        {
            this->tree = vector<ui>(size, 0);
            this->size = size;
        }
        void add(const ui& node, const ui& val)
        {
            const ui n = this->size;
            int64_t k = node;
            //cout << "Request: " << node << endl;
            while (k <= n)
            {
                this->tree[k] += val;
                this->tree[k] %= ANS_MOD;
                //cout << "updating " << k << endl;
                k += (k & (-k)); // iterating through all nodes having less than or equal heights that are to the right of the current node.
            }
        }
        ui get(const ui& node)
        {
            ui ans = 0;

            int64_t k = node;
            const ui n = this->size;
            while (k > 0)
            {
                ans += this->tree[k];
                ans %= ANS_MOD;
                k -= (k & (-k));
            }

            return ans;
        }
};

ui calculate()
{
    vector<FenwickTreePrefixSum> acc_sum = vector<FenwickTreePrefixSum>(K+1, FenwickTreePrefixSum(SEQ_MOD+1));
    vector<FenwickTreePrefixSum> acc_count = vector<FenwickTreePrefixSum>(K+1, FenwickTreePrefixSum(SEQ_MOD+1));

    auto seq = SequenceGenerator();
    for (auto i = 0; i < M; i++)
    {
        if (i % 10000 == 0)
            cout << i << endl;
        ui curr = seq.get_next();
        acc_sum[1].add(curr, curr);
        acc_count[1].add(curr, 1);
        for (auto length = 2; length <= K; length++)
        {
            ui prev_sum = acc_sum[length-1].get(curr-1);
            ui prev_count = acc_count[length-1].get(curr-1);
            acc_sum[length].add(curr, prev_sum + prev_count * curr);
            acc_count[length].add(curr, prev_count);
            //cout << "length: " << length << "; curr: " << curr << "; acc_sum: " << acc_sum[length].get(curr) << "; acc_count: " << acc_count[length].get(curr) << endl;
        }
    }

    return acc_sum[K].get(SEQ_MOD);
}

int main()
{
    //FenwickTreePrefixSum sum(SEQ_MOD+1);
    //sum.update(9, 10);
    ui ans = calculate();
    cout << ans << endl;
    return 0;
}
