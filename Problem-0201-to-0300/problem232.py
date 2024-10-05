# Problem: https://projecteuler.net/problem=232

from decimal import *

getcontext().prec = 20

N = 100

# return the probability of reaching the score of p1_score, p2_score after player 2 made a move, and then player 1 made a move
# i.e. P(next scores given current scores)
# i.e. P(p1 = p1_score, p2 = p2_score | prev_p1_score, prev_p2_score are known)
# hence if p2_score >= 100 then p2 wins even if p1_score = 100
def dfs(p1_score, p2_score, cache):
    if p2_score >= 100:
        return Decimal(1.0) # for backpropagating the probability

    if p1_score >= 100:
        return Decimal(0.0)

    if (p1_score, p2_score) in cache:
        return cache[(p1_score, p2_score)]

    p2_bet = 1

    p_winning_p2_bet = Decimal(0.5)

    p_winning_p1_bet = Decimal(0.5)

    p2_best_win_probability = Decimal(0.0)

    while True:

        # If neither p1 wins nor p2 wins, we return to the same p1_score, p2_score state.
        # Using Markov process, we have that, after being stuck at this state N times,
        # and as N approaching infinity, the probability of being stuck at this state N+1 times approaches zero
        # and
        # P(p1 wins, p2 wins) = P(p1 wins) * P(p2 wins) / (1 - (1 - p1 wins) * (1 - p2 wins))
        # P(p1 wins, p2 loses) = P(p1 wins) * P(p2 loses) / (1 - (1 - p1 wins) * (1 - p2 wins))
        # P(p1 loses, p2 wins) = P(p1 loses) * P(p2 wins) / (1 - (1 - p1 wins) * (1 - p2 wins))
        normalizing_factor = 1 / (1 - (1 - p_winning_p1_bet) * (1 - p_winning_p2_bet) )

        p2_win_probability = p_winning_p1_bet       * p_winning_p2_bet       * dfs(p1_score + 1, p2_score + p2_bet, cache) \
                           + (1 - p_winning_p1_bet) * p_winning_p2_bet       * dfs(p1_score, p2_score + p2_bet, cache) \
                           + p_winning_p1_bet       * (1 - p_winning_p2_bet) * dfs(p1_score + 1, p2_score, cache)

        p2_win_probability = normalizing_factor * p2_win_probability

        p2_best_win_probability = max(p2_best_win_probability, p2_win_probability)    

        if p2_score + p2_bet > N:
            break

        p2_bet = p2_bet * 2
        p_winning_p2_bet = p_winning_p2_bet * Decimal(0.5)

    if not (p1_score, p2_score) in cache:
        cache[(p1_score, p2_score)] = p2_best_win_probability

    return cache[(p1_score, p2_score)]


if __name__ == "__main__":
    ans = 0

    cache = {}

    # Since dfs only concerns p2 making a move and then p1 making a move
    # we need to mutually make p1 first move
    ans = Decimal("1/2") * dfs(0, 0, cache) + Decimal("1/2") * dfs(1, 0, cache)

    print(ans)