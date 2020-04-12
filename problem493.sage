# Question: https://projecteuler.net/problem=493

# Let X_i be the event that at least one ball of color i appear among 20 picked balls.
# There are C(70,20) combinations of picking 20 balls, and C(60,20) combinations of picking 20 balls without color i.
# So, P(X_i) = 1 - C(60,20) / C(70,20) and E(X_i) = P(X_i)
# E(sum(X_i)) = sum(E(X_i)) = 7 * P(X_i) = 7 * (1 - C(60,20) / C(70,20))

print(7 - 7 * binomial(60,20) / binomial(70,20))
