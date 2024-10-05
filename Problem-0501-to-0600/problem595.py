# Question: https://projecteuler.net/problem=595

"""
- Essentially, if we have 'k' fragments, then we shuffle and get 'p' fragments instead, then we have,
  . p <= k
  . P[next_n_fragments = p | curr_n_fragments = k] = (number of permutations of p numbers with k fragments) / (number of permutations of p numbers)
- How do we get the distribution of p numbers w.r.t number of fragments?
  . the implementation is in get_permutation_fragment_count_distributions()
  . the basic idea is that, if we know the distribution of n numbers, then we add 1 new number such that the new number is larger than all numbers in the previous distribution.
- We can encode the number of fragments as state.
- We can use absorbing Markov chain to find the expected number of steps.
  . https://en.wikipedia.org/wiki/Absorbing_Markov_chain#Expected_number_of_steps
"""

from sage.all import *

N = 52

def get_permutation_fragment_count_distributions(max_length):
    dist = [{j: 0 for j in range(k+1)} for k in range(max_length+1)]
    
    dist[0] = {0: 0}
    dist[1] = {1: 1}
    dist[2] = {1: QQ(1/2), 2: QQ(1/2)}

    for n in range(3, max_length+1):
        prev_length = n-1
        for prev_fragment_count, prev_fragment_probability in dist[prev_length].items():
            # this is to make sure that we counted all possibilities
            count_check = 0

            # for each of the previous permutation, there are n places to place the new number
            # e.g., if the prev permutation is of length 5, then there'll be 6 places to place the new number
            one_permutation_probability = QQ(prev_fragment_probability) / n
            
            # try appending the new number (which is the largest number) to the front of the current permutation
            # it always results in one more fragment
            dist[n][prev_fragment_count+1] += one_permutation_probability
            count_check += 1

            # try to add the new number to the middle of each fragment
            # the number of "middle" places is n - fragment_count
            # since we add the largest number to the middle, it will always create two more fragments
            n_middle_places = prev_length - prev_fragment_count
            if n_middle_places > 0:
                dist[n][prev_fragment_count+2] += n_middle_places * one_permutation_probability
            count_check += n_middle_places

            # try adding the new number to the right of each fragment
            # there are fragment_count places that are to the right of a fragment
            # since we add the largest number to the right,
            #  - there is exactly one fragment that the new number won't create new fragments.
            #    e.g. adding 6 to [4,5] [1,2,3] -> [4,5,6] [1,2,3]
            #  - in all other places, the new number will create one new fragment.
            #    e.g. adding 6 to [4,5] [1,2,3] -> [4,5] [1,2,3,6]
            n_right_places = prev_fragment_count
            if n_right_places > 1:
                dist[n][prev_fragment_count+1] += (n_right_places - 1) * one_permutation_probability
            dist[n][prev_fragment_count] += one_permutation_probability
            count_check += n_right_places

            assert(count_check == prev_length+1)

    return dist

if __name__ == "__main__":
    distributions = get_permutation_fragment_count_distributions(N)

    get_state_index = lambda n_fragments: n_fragments - 1

    absorbing_state = get_state_index(1)

    initial_S = matrix(QQ, nrows=N, ncols=1)
    for n_fragments in range(1, N+1):
        initial_S[get_state_index(n_fragments)] = distributions[N][n_fragments]
    assert(sum(initial_S)[0] == 1)

    T = matrix(QQ, nrows=N, ncols=N)
    for curr_n_fragments in range(1, N+1):
        for next_n_fragments in range(1, curr_n_fragments+1):
            T[get_state_index(next_n_fragments), get_state_index(curr_n_fragments)] += distributions[curr_n_fragments][next_n_fragments]

    T = T.transpose()
    I = matrix.identity(QQ, N)
    I[absorbing_state, 0] = 0

    Inv = (I - T).inverse()

    One = matrix(QQ, nrows=N, ncols=1)
    for n_fragments in range(1, N+1):
        One[get_state_index(n_fragments), 0] = 1

    E_steps = Inv * One

    ans = 0
    for n_fragments in range(1, N+1):
        ans += E_steps[get_state_index(n_fragments), 0] * initial_S[get_state_index(n_fragments), 0]

    print(float(ans)+1)
