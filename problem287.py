# Question: https://projecteuler.net/problem=287

N = 24
alpha = 2**(N-1)
beta = 2**(2*N-2)

inside_circle = lambda x, y: (x-alpha)**2  + (y-alpha)**2 - beta <= 0
different_halves = lambda p1, p2: (p1 <= alpha - 1) != (p2 <= alpha - 1)

def same_color(x1, y1, x2, y2):
    # There are 3 cases:
    # Case 1 -> all points are inside the circle
    # Case 2 -> all points are outside the circle, then the rectangle has one color if either (x1 and x2) are in same half of the circle and (y1 and y2) are in same half of the circle
    # Case 3 -> otherwise, the region has 2 colors
    inside_circles = [inside_circle(x1, y1), inside_circle(x1, y2), inside_circle(x2, y1), inside_circle(x2, y2)]
    n_insides = sum(inside_circles)
    return (n_insides == 4) \
           or (n_insides == 0) and (not different_halves(x1, x2) and not different_halves(y1, y2))

def get_quarters(x1, y1, x2, y2):
    # quarter 1 -> top left
    # quarter 2 -> top right
    # quarter 3 -> bottom left
    # quarter 4 -> bottom right
    mid_x = (x1 + x2) // 2
    mid_y = (y1 + y2) // 2
    return [(x1       , mid_y + 1, mid_x, y2), 
            (mid_x + 1, mid_y + 1, x2   , y2), 
            (x1       , y1       , mid_x, mid_y), 
            (mid_x + 1, y1       , x2   , mid_y)]

def compressed_length(x1, y1, x2, y2):
    # base case
    if x1 == x2 and y1 == y2:
        return 2 # 10 or 11
    # the whole subregion has the same color if it does not overlap the circle
    if same_color(x1, y1, x2, y2):
        return 2 # 10 or 11
    # otherwise, we have to calculate the length of each sub-region
    quarters = get_quarters(x1, y1, x2, y2)
    quarter_lengths = [compressed_length(x1, y1, x2, y2) for x1, y1, x2, y2 in quarters]
    return 1 + sum(quarter_lengths)

print(compressed_length(0, 0, 2**N - 1, 2**N - 1))
