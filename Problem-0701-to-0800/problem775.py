# Problem: https://projecteuler.net/problem=775
#
# Constructing the optimal shape: http://oeis.org/A193416
#
# TODO: visualize the solution

N = 10**16
MOD = 10**9 + 7

def partial_alpha(n, k):
    if k == 0:
        return 0
    ans = 0
    ans += 4*k
    j = 1
    step = 1
    step_count = 1
    last_possible_j = n**2-1 if n > 1 else 0
    last_possible_j = min(last_possible_j, k)

    while j <= last_possible_j:
        ans += 2 * (k - j)
        j += step
        step_count += 1
        if step_count == 2:
            step += 1
            step_count = 0
    return ans

def partial_beta(n, k):
    if k == 0:
        return 0
    ans = 0
    ans += 4*k
    j = 1
    step = 1
    step_count = 1
    last_possible_j = n**2
    last_possible_j = min(last_possible_j, k)
    while j <= last_possible_j:
        ans += 2 * (k - j)
        j += step
        step_count += 1
        if step_count == 2:
            step += 1
            step_count = 0
    return ans

def alpha(n, k): # total surface area added by adding an n x n layer to all boxes with k cubes to be filled
    return 4*n*k - (4*(n**3) - 3*(n**2) - n)//3

def beta(n, k): # total surface area added by adding an n x (n+1) layer to all boxes with k cubes to be filled
    return (4*n+2)*k - (4*(n**3) + 3*(n**2) - n)//3

# a box -> a shape formed by a set of cubes
# count the added surface area contributed by the newly added cubes (added at layer (n+1)^th) across all boxes having more than N - n_remaining cubes
def count(n, n_remaining, mod):
    ans = 0

    # adding an (n x n) layer
    n_to_be_filled = n*n
    if n_to_be_filled > n_remaining:
        ans += partial_alpha(n, n_remaining)
        n_remaining = 0
    else:
        ans += alpha(n, n_remaining)
        n_remaining -= n_to_be_filled

    # adding an (n x (n+1)) layer
    n_to_be_filled = n*(n+1)
    if n_to_be_filled > n_remaining:
        ans += partial_beta(n, n_remaining)
        n_remaining = 0
    else:
        ans += beta(n, n_remaining)
        n_remaining -= n_to_be_filled

    # adding an ((n+1) x (n+1)) layer
    n_to_be_filled = (n+1) * (n+1)
    if n_to_be_filled > n_remaining:
        ans += partial_alpha(n+1, n_remaining)
        n_remaining = 0
    else:
        ans += alpha(n+1, n_remaining)
        n_remaining -= n_to_be_filled
    
    return ans, n_remaining

def G(n):
    ans = 0

    surface_area_wrapping_individual_cube = 6 * (n * (n+1) // 2)

    surface_area_optimal = 0
    surface_area_optimal += 6*n # layer 1
    n_remaining = n - 1
    layer = 1
    while n_remaining > 0:
        delta, n_remaining = count(layer, n_remaining, MOD)
        surface_area_optimal += delta
        layer += 1

    ans = surface_area_wrapping_individual_cube - surface_area_optimal

    return ans

if __name__ == "__main__":
    print(G(N) % MOD)
