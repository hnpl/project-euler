// https://projecteuler.net/problem=491

/*
    Divisibility test for 11: sum of digit in odd positions - sum of digit in even positions is divisible by 11.
    Double pandigital -> each of [0..9] appears twice.
    Suppose we have a multiset of {0, 0, 1, 1, 2, 2, ..., 9, 9}, and we pick 10 of them and put the 10 digits to odd positions.
                            bit    0  1  2  3  4  5      18 19
                            bit[i] = 1 -> corresponding digit is in even position
                            bit[i] = 0 -> corresponding digit is in odd position
        - We can iterate thru all sets of 20 of bits with 10 bits being set using this algorithm. (https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation)
        - If 2 bits of digit i is set -> there are 2 i-digit's in even positions.
             1 bit          "                "     1   "
             0 bits         "                "     0   "
        - Note that we can have patterns '01' and '10' for each digit. To avoid double counting, we hash the frequencies of the digits, so that both '01' and '10' map to 1.
    Suppose we have freq_odd such that freq_odd[i] = q means digit i appear q times in the odd positions.
    We have 10 odd positions -> we can use this theorem to calculate the number of ways to place the 10 digits in to 10 positions with there are digits that are the same: https://en.wikipedia.org/wiki/Multinomial_theorem.
    #ways_odd = factorial(10)/product(factorial(freq_odd[i]))
    Similarly, we have 10 even positions, but we don't want to place 0-digit to the first position.
    So there are 9 ways to place the first digit, and 9! to fill the next 9 digits to 9 position.
    Using multinomial theorem, #ways_even = 9*factorial(9)/product(factorial(freq_even[i]))
*/

#include<iostream>
#include<bitset>
#include<vector>
#include<unordered_set>

typedef int64_t int64;

using namespace std;

const int64 all_digit_sum = (0 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9) * 2;

//https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
int64 next_pattern(int64 current)
{
    int64 next;
    int64 t = current | (current - 1);
    next = (t + 1) | (((~t & -~t) - 1) >> (__builtin_ctz(current) + 1));
    return next;
}

int64 factorial(int64 n)
{
    int64 ans = 1;
    for (int64 i = 2; i <= n; i++)
        ans = ans * i;
    return ans;
}

int64 calculate_ways(vector<int64> digit_freq, bool count_zero_first_digit)
{
    int64 ans = factorial(10);
    if (!count_zero_first_digit)
        ans = 9 * factorial(9);
    for (auto freq: digit_freq)
        if (freq == 2)
            ans = ans / 2;
    return ans;
}

int main()
{
    int64 ans = 0;
    int64 start_pattern = 0x003FF;
    int64 end_pattern = 0xFFC00;
    unordered_set<int64> even_bit_digit_freq_set;

    for (int64 pattern = start_pattern; pattern <= end_pattern;)
    {
        bitset<20> bits = bitset<20>(pattern);
        int64 odd_pos_digit_sum = 0;
        int64 even_pos_digit_sum = 0;
        for (int64 i = 0; i < 20; i++)
            even_pos_digit_sum = even_pos_digit_sum + (i/2) * bits[i];
        int64 sum = 0;
        for (int64 i = 0; i < 20; i++)

        odd_pos_digit_sum = all_digit_sum - even_pos_digit_sum;
        if ((odd_pos_digit_sum - even_pos_digit_sum) % 11 == 0)
        {
            int64 even_bit_digit_freq_hash = 0;
            for (int64 i = 0; i < 20; i+=2)
            {
                even_bit_digit_freq_hash = even_bit_digit_freq_hash << 2;
                even_bit_digit_freq_hash = even_bit_digit_freq_hash + bits[i] + bits[i+1];
            }
            if (even_bit_digit_freq_set.find(even_bit_digit_freq_hash) == even_bit_digit_freq_set.end())
            {
                even_bit_digit_freq_set.insert(even_bit_digit_freq_hash);
                vector<int64> odd_pos_digit_freq = vector<int64>(10, 0);
                vector<int64> even_pos_digit_freq = vector<int64>(10, 0);
                for (int64 i = 0; i < 20; i++)
                    if (bits[i] == 0)
                        odd_pos_digit_freq[i/2] = odd_pos_digit_freq[i/2] + 1;
                    else
                        even_pos_digit_freq[i/2] = even_pos_digit_freq[i/2] + 1;
                ans = ans + calculate_ways(odd_pos_digit_freq, true) * calculate_ways(even_pos_digit_freq, false);
            }
        }
        pattern = next_pattern(pattern);
    }
    cout << ans << endl;
    return 0;
}
