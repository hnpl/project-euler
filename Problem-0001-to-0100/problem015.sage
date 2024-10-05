# Question: https://projecteuler.net/problem=15

# Assume (x,y) is the current position. Since we can only move rightward or downward, each step we either have x+1 or y+1, so we need exactly 20 rightward moves and 20 downward moves in order to get to the bottom right of the grid.
# Let 'r' represent moving rightward, 'd' represent moving downward. So the moving sequence could be 'rrdddrrrrrdd...'.
# The problem can be rephrased as how many arrangments are there of 20 'r' and 20 'd', or even simpler, how many ways to put 20 'r' to 40 slots? Well, it's C(40, 20).

binomial(40, 20)


