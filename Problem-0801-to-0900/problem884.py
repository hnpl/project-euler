from math import floor

N = 10**17

def sum_i_j(i, j):
    offset = i
    count = j - i + 1
    return offset * count + count * (count - 1) // 2

cache = {}
# initialize the all-one cases
for i in range(0, 2**3):
    for j in range(i, 2**3):
        cache[(i,j)] = sum_i_j(i, j)

#solve_stack = []
def solve(start, end):
    #solve_stack.append((start, end))
    #print((start, end), "solve_stack", solve_stack)
    if (start, end) in cache:
        #print((start, end), "hits")
        #del solve_stack[-1]
        return cache[(start, end)]

    ans = 0
    k_start = floor(start**(1/3))
    k_end = floor(end**(1/3))
    # rounding issue
    if (k_start+1)**3 == start:
        k_start += 1
    if (k_end+1)**3 == end:
        k_end += 1
    num_subranges = end // (k_end**3)
    #print("Split", (0, num_subranges * (k_end**3)-1), (0, end-(num_subranges * (k_end**3))))
    ans += (num_subranges * (num_subranges - 1) // 2) * (k_end**3) + num_subranges * solve(0, k_end**3 - 1)
    ans += num_subranges * (end - num_subranges * (k_end**3) + 1)
    ans += solve(0, end - (num_subranges * (k_end**3)))

    #print("Adding", (start, end), "to cache, value = ", ans)
    cache[(start, end)] = ans
    #del solve_stack[-1]
    return ans

if __name__ == "__main__":
    k_upperbound = floor(N**(1/3))
    # populate cache
    # doing this prevents stack overflow issues
    for k in range(2, k_upperbound):
        solve(0, k**3-1)
    ans = solve(0, N-1)
    print(ans)
