# https://projecteuler.net/problem=113

# Mapping this problem to the lattice path problem:
#   - For increasing numbers, the number does not contain digit-0.
#     Let the horizontal axis represent the i-th digit.
#     Let the vertical axis represent the value of the i-th digit.
#     Let only consider a path consisting of the rightward and the upward
# moves.
#     Each path from (null,null) to (9,100) represents a unique number, where
# the i-th digit of the number is the point (a, i) that 'a' is the smallest
# among (*, i). In other words, (a, i) is the receiving end of the edge of
# ((a-1, i), (a,i)).   
#     The number of increasing numbers is the number of path from (null, 1)
# to (9, 100).
#     digit null means that we don't place any digit (to represent numbers
# of fewer than 100 digits).
#     null-th digit is a placeholder for making an edge to the first digit.
#
#       9 .
#       8 .
#       7 .
#       6 .
#       5 .
#       4 .
#       3 .
#       2 .
#       1 .
#    null .  . . . . .  ...  .
#       null 1 2 3 4 5 6    100
#
#    Note that we have a number that only consists of 'null' digits. This is 
# an invalid number.
#    There are C(100+9, 9) paths -> C(100+9, 9) - 1 increasing numbers.
#   - We use a similar strategy to count the number of decreasing numbers.
#       0 .
#       1 .
#       2 .
#       3 .
#       4 .
#       5 .
#       6 .
#       7 .
#       8 .
#       9 .
#    null .  . . . . .  ...  .
#       null 1 2 3 4 5 6    100
#
#     There are C(100+10, 10) paths -> C(100+10, 10) - 1 increasing numbers.
#
#   - Note that both strategies generate both increasing-decreasing numbers,
# e.g. 111, 22222, 3333333. There are 10*100 of them.
#
# So, the answer is C(100+9, 9) - 1 + C(100+10, 10) - 1 - 100*10
#                 = 51161058134250
