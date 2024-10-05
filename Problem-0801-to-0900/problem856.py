# Question: https://projecteuler.net/problem=856

"""
    . We divide the remaining cards into 4 piles,
        . pile num1s: { card C | for all C: num_cards_across_all_piles(rank(C)) = 1 }; i.e., a pile of (cards of) ranks where each rank only has 1 card across all piles.
        . pile num2s: { card C | for all C: num_cards_across_all_piles(rank(C)) = 2 }
        . pile num3s: { card C | for all C: num_cards_across_all_piles(rank(C)) = 3 }
        . pile num4s: { card C | for all C: num_cards_across_all_piles(rank(C)) = 4 }

    . State = (number of ranks having 1 card, number of ranks having 2 cards, number of ranks having 3 cards, number of ranks having 4 cards, which pile that the rank of the previous card is now in)
"""

class State:
    def __init__(self, num1s, num2s, num3s, num4s, same_rank_in_pile):
        self.num1s = num1s
        self.num2s = num2s
        self.num3s = num3s
        self.num4s = num4s
        self.same_rank_in_pile = same_rank_in_pile
        self.hash = self.get_hash()
    def get_hash(self):
        return (self.same_rank_in_pile << 16) | (self.num1s << 12) | (self.num2s << 8) | (self.num3s << 4) | (self.num4s << 0)
    def get_num_remaining_cards(self):
        return self.num1s + self.num2s * 2 + self.num3s * 3 + self.num4s * 4
    def get_next_states(self):
        num_remaining_cards = self.get_num_remaining_cards()
        next_states = []

        if self.num4s > 0:
            # picking 1 card from the num4s pile
            is_ending_state = False # the card of a rank that has never been picked before
            probability = self.num4s * 4 / num_remaining_cards
            next_state_hash = State(self.num1s, self.num2s, self.num3s + 1, self.num4s - 1, 3).get_hash()
            next_states.append((next_state_hash, probability, is_ending_state))

        # picking 1 card from the num3s pile
        if self.num3s > 0:
            # - picking 1 card from the num3s pile such that the new card has same rank as the previous one
            if self.same_rank_in_pile == 3:
                is_ending_state = True
                probability = 3 / num_remaining_cards
                next_state_hash = 0 # does not matter
                next_states.append((next_state_hash, probability, is_ending_state))
            # - picking 1 card from the num3s pile such that the new card does not have the same rank as the previous one
            is_ending_state = False
            probability = self.num3s * 3 / num_remaining_cards
            if self.same_rank_in_pile == 3:
                probability = (self.num3s - 1) * 3 / num_remaining_cards # excluding the cards having the same rank as the previous draw
            next_state_hash = State(self.num1s, self.num2s + 1, self.num3s - 1, self.num4s, 2).get_hash()
            next_states.append((next_state_hash, probability, is_ending_state))

        # picking 1 card from the num2s pile
        if self.num2s > 0:
            # - picking 1 card from the num2s pile such that the new card has same rank as the previous one
            if self.same_rank_in_pile == 2:
                is_ending_state = True
                probability = 2 / num_remaining_cards
                next_state_hash = 0 # does not matter
                next_states.append((next_state_hash, probability, is_ending_state))
            # - picking 1 card from the num2s pile such that the new card does not have the same rank as the previous one
            is_ending_state = False
            probability = self.num2s * 2 / num_remaining_cards
            if self.same_rank_in_pile == 2:
                probability = (self.num2s - 1) * 2 / num_remaining_cards # excluding the cards having the same rank as the previous draw
            next_state_hash = State(self.num1s + 1, self.num2s - 1, self.num3s, self.num4s, 1).get_hash()
            next_states.append((next_state_hash, probability, is_ending_state))

        # picking 1 card from the num1s pile
        if self.num1s > 0:
            # - picking 1 card from the num1s pile such that the new card has same rank as the previous one
            if self.same_rank_in_pile == 1:
                is_ending_state = True
                probability = 1 / num_remaining_cards
                next_state_hash = 0 # does not matter
                next_states.append((next_state_hash, probability, is_ending_state))
            # - picking 1 card from the num1s pile such that the new card does not have the same rank as the previous one
            is_ending_state = False
            probability = self.num1s * 1 / num_remaining_cards
            if self.same_rank_in_pile == 1:
                probability = (self.num1s - 1) * 1 / num_remaining_cards # excluding the card having the same rank as the previous draw
            next_state_hash = State(self.num1s - 1, self.num2s, self.num3s, self.num4s, 0).get_hash()
            next_states.append((next_state_hash, probability, is_ending_state))
        return next_states

def state_from_hash(hash):
    num4s = hash & 0xF
    num3s = (hash >> 4) & 0xF
    num2s = (hash >> 8) & 0xF
    num1s = (hash >> 12) & 0xF
    same_rank_in_pile = hash >> 16
    return State(num1s, num2s, num3s, num4s, same_rank_in_pile)

def solve(num_draws):
    expectation = 0.0
    initial_state_hash = State(0, 0, 0, 13, 0).get_hash()
    curr_states = { initial_state_hash: 1.0 }
    prev_states = {}
    for i in range(num_draws):
        num_card_drawn = i + 1
        curr_states, prev_states = {}, curr_states
        for prev_state_hash, prev_state_probability in prev_states.items():
            prev_state = state_from_hash(prev_state_hash)
            for next_state_hash, next_state_probability_given_prev_state, is_ending_state in prev_state.get_next_states():
                next_state_probability = prev_state_probability * next_state_probability_given_prev_state
                if is_ending_state or num_card_drawn == num_draws: # the game stops when two consecutive cards are of the same rank or all cards were drawn
                    expectation += next_state_probability * num_card_drawn
                else:
                    if not next_state_hash in curr_states:
                        curr_states[next_state_hash] = 0.0
                    curr_states[next_state_hash] += next_state_probability
        print(i, len(curr_states))
    return expectation

if __name__ == "__main__":
    ans = solve(52)
    print(ans)
