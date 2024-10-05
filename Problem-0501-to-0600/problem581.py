# Problem: https://projecteuler.net/problem=581

"""
    Basically find all pairs of (n, n+1) such that both n and n+1 are 47-smooth.
    Generate all 47-smooth numbers k and check whether k-1 or k+1 is in the 47-smooth number set.
"""

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

n_upperbound = 10**15 # increase this until getting the correct answer :D

def dfs(depth, target_depth, product, ans, smooth_set, primes):
    if depth == target_depth:
        if product-1 in smooth_set:
            ans.add(product-1)
        if product+1 in smooth_set:
            ans.add(product)
        smooth_set.add(product)
    else:
        base = primes[depth]
        exp = 0
        while True:
            next_product = product * (base ** exp)
            if next_product > n_upperbound:
                return
            dfs(depth+1, target_depth, next_product, ans, smooth_set, primes)
            exp += 1

if __name__ == "__main__":
    ans = set()
    dfs(0, len(primes), 1, ans, set(), primes)
    for x in sorted(list(ans)):
        print(x)
    print(sum(ans))
