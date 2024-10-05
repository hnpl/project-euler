# Problem: https://projecteuler.net/problem=687

"""
    . Define a state as a composition of the following:
        . n_remaining_count_freq as
            n_remaining_count_freq[k] = m means that there are m ranks having k cards of the same rank that haven't been picked
        . n_most_recent_card_rank_remaining: the number of remaining cards having the same rank as the most recent picked card
        . most_recent_card_rank_is_perfect: whether the most recent card's rank is perfect
        . n_perfect_ranks: the number of perfect ranks
    . Starting from no cards having been picked, picking one card at a time and iterating through all generated states until all 52 cards have been picked.
"""

N = 52
N_RANKS = 13
N_CARDS_PER_RANK = 4
INVALID = N_RANKS + 1
primes = {2, 3, 5, 7, 11, 13}

def mask(n):
    return 2**n - 1

def factorial(n):
    ans = 1

    for k in range(2, n+1):
        ans = ans * k

    return ans


class State:
    def __init__(self, n_remaining_count_freq, n_most_recent_card_rank_remaining, most_recent_card_rank_is_perfect, n_perfect_ranks):
        self.n_remaining_count_freq = n_remaining_count_freq[:]
        self.n_most_recent_card_rank_remaining = n_most_recent_card_rank_remaining
        self.most_recent_card_rank_is_perfect = most_recent_card_rank_is_perfect
        self.n_perfect_ranks = n_perfect_ranks
        self.hash = State.encode(n_remaining_count_freq, n_most_recent_card_rank_remaining, most_recent_card_rank_is_perfect, n_perfect_ranks)

    def describe_state(self):
        print(self.n_remaining_count_freq, "most_recent_remaining:", self.n_most_recent_card_rank_remaining, "most_recent_is_perfect:", self.most_recent_card_rank_is_perfect, "n_perfect:", self.n_perfect_ranks)

    def get_initial_state():
        n_remaining_count_freq = [0] * (N_CARDS_PER_RANK + 1)
        n_remaining_count_freq[N_CARDS_PER_RANK] = N_RANKS
        n_most_recent_card_rank_remaining = 0
        most_recent_card_rank_is_perfect = 0
        n_perfect_ranks = N_RANKS
        state = State(n_remaining_count_freq, n_most_recent_card_rank_remaining, most_recent_card_rank_is_perfect, n_perfect_ranks)
        return state
    def encode(n_remaining_count_freq, n_most_recent_card_rank_remaining, most_recent_card_rank_is_perfect, n_perfect_ranks):
        h = 0
        msb_index = 0

        for i in range(N_CARDS_PER_RANK+1):
            h = h | (n_remaining_count_freq[i] << msb_index)
            msb_index += 4

        h = h | (n_most_recent_card_rank_remaining << msb_index)
        msb_index += 3

        h = h | (most_recent_card_rank_is_perfect << msb_index)
        msb_index += 1

        h = h | (n_perfect_ranks << msb_index)
        msb_index += 4

        return h

    def decode(state_hash):
        n_remaining_count_freq = [0] * (N_CARDS_PER_RANK + 1)
        n_most_recent_card_rank_remaining = 0
        most_recent_card_rank_is_perfect = 0
        n_perfect_ranks = 0

        for i in range(N_CARDS_PER_RANK + 1):
            n_remaining_count_freq[i] = state_hash & mask(4)
            state_hash >>= 4
        
        n_most_recent_card_rank_remaining = state_hash & mask(3)
        state_hash >>= 3

        most_recent_card_rank_is_perfect = state_hash & mask(1)
        state_hash >>= 1

        n_perfect_ranks = state_hash & mask(4)
        state_hash >>= 4

        return n_remaining_count_freq, n_most_recent_card_rank_remaining, most_recent_card_rank_is_perfect, n_perfect_ranks

    def get_next_states(self, n_remaining_cards):
        next_states = []

        n_imperfect_rank_cards = n_remaining_cards
        for count in range(N_CARDS_PER_RANK+1):
            n_imperfect_rank_cards -= count * self.n_remaining_count_freq[count]
        if self.most_recent_card_rank_is_perfect:
            n_imperfect_rank_cards -= self.n_most_recent_card_rank_remaining

        # picking one of the perfect ranks that is not the most recent rank
        next_n_remaining_count_freq = self.n_remaining_count_freq[:]
        # --- put the card back, but remember not to choose this card
        if self.most_recent_card_rank_is_perfect:
            next_n_remaining_count_freq[self.n_most_recent_card_rank_remaining] += 1
        for count in range(1, N_CARDS_PER_RANK+1):
            n_choices = next_n_remaining_count_freq[count]
            if count == self.n_most_recent_card_rank_remaining:
                n_choices -= 1 # don't choose the card that has just been put back
            if n_choices <= 0:
                continue
            n_choices = n_choices * count
            next_n_remaining_count_freq[count] -= 1
            next_n_most_recent_card_rank_remaining = count - 1
            next_most_recent_card_rank_is_perfect = 1
            next_n_perfect_ranks = self.n_perfect_ranks
            next_state = State(next_n_remaining_count_freq, next_n_most_recent_card_rank_remaining, next_most_recent_card_rank_is_perfect, next_n_perfect_ranks)
            next_states.append((next_state, n_choices))
            next_n_remaining_count_freq[count] += 1

        # picking one of the imperfect ranks
        if n_imperfect_rank_cards > 0:
            n_choices = n_imperfect_rank_cards
            next_n_most_recent_card_rank_remaining = 0
            next_most_recent_card_rank_is_perfect = 0
            next_n_perfect_ranks = self.n_perfect_ranks
            next_state = State(next_n_remaining_count_freq, next_n_most_recent_card_rank_remaining, next_most_recent_card_rank_is_perfect, next_n_perfect_ranks)
            next_states.append((next_state, n_choices))

        # put the card that has been put back out
        if self.most_recent_card_rank_is_perfect:
            next_n_remaining_count_freq[self.n_most_recent_card_rank_remaining] -= 1

        # picking the same rank as the most recent card
        # if the most recent card rank is perfect, it's no longer perfect
        if self.most_recent_card_rank_is_perfect and self.n_most_recent_card_rank_remaining > 0:
            n_choices = self.n_most_recent_card_rank_remaining
            next_n_most_recent_card_rank_remaining = 0
            next_most_recent_card_rank_is_perfect = 0
            next_n_perfect_ranks = self.n_perfect_ranks - 1
            next_state = State(next_n_remaining_count_freq, next_n_most_recent_card_rank_remaining, next_most_recent_card_rank_is_perfect, next_n_perfect_ranks)
            next_states.append((next_state, n_choices))


        #print("From:", end = " ")
        #self.describe_state()
        #for next_state, count in next_states:
        #    print("To: ({} ways)".format(count), end = " ")
        #    next_state.describe_state()

        return next_states

    def get_n_perfect_ranks(self):
        return self.n_perfect_ranks

if __name__ == "__main__":
    ans = 0

    states = [{}, {}]

    state_hash_state_map = {}

    prev_index, curr_index = 0, 1

    initial_state = State.get_initial_state()
    initial_state_hash = initial_state.hash
    states[curr_index][initial_state_hash] = 1
    state_hash_state_map[initial_state_hash] = initial_state

    for n_picked_cards in range(N):
    #for n_picked_cards in range(4):
        n_remaining_cards = N_RANKS * N_CARDS_PER_RANK - n_picked_cards

        curr_index = 1 - curr_index
        prev_index = 1 - prev_index

        states[curr_index] = {}

        prev_states = states[prev_index]
        curr_states = states[curr_index]

        for prev_state_hash, prev_state_count in prev_states.items():
            prev_state = state_hash_state_map[prev_state_hash]
            for next_state, next_state_count in prev_state.get_next_states(n_remaining_cards):
                next_state_hash = next_state.hash
                if not next_state_hash in state_hash_state_map:
                    state_hash_state_map[next_state_hash] = next_state
                if not next_state_hash in curr_states:
                    curr_states[next_state_hash] = prev_state_count * next_state_count
                else:
                    curr_states[next_state_hash] += prev_state_count * next_state_count
    total = 0
    for state_hash, state_count in states[curr_index].items():
        state = state_hash_state_map[state_hash]
        if state.get_n_perfect_ranks() in primes:
            ans += state_count
        total += state_count

    print(ans * 1.0 / factorial(52))

