# Problem: https://projecteuler.net/problem=581

"""
    Basically find all pairs of (n, n+1) such that both n and n+1 are 47-smooth.
"""

import sympy
from sympy.solvers.diophantine.diophantine import diop_DN

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]

def is_47_smooth(n):
    for prime in primes:
        while n % prime == 0:
            n = n // prime
    if n > 1:
        return False
    return True

# https://en.wikipedia.org/wiki/Pell%27s_equation#Additional_solutions_from_the_fundamental_solution
def generate_DN_solutions_from_the_fundamental_solution(D, N, x_1, y_1, max_k):
    ans = [(x_1, y_1)]
    for k in range(2, max_k+1):
        x_k = x_1 * ans[-1][0] + N * y_1 * ans[-1][1]
        y_k = x_1 * ans[-1][1] + y_1 * ans[-1][0]
        ans.append((x_k, y_k))
    return ans

def get_DN_fundamental_solution(D, N):
    return diop_DN(D, N)

x =  0
def dfs(depth, target_depth, product, ans, primes):
    # https://en.wikipedia.org/wiki/St%C3%B8rmer%27s_theorem#The_procedure
    if depth == target_depth:
        global x
        x = x + 1
        print(x, product)
        if product == 2:
            return
        for x_1, y_1 in diop_DN(2*product, 1):
            if not is_47_smooth((x_1-1)//2) or not is_47_smooth((x_1+1)//2): 
                continue
            solutions = generate_DN_solutions_from_the_fundamental_solution(2*product, 1, x_1, y_1, max(3, (47+1)//2))
            for x_k, y_k in solutions:
                if is_47_smooth((x_k-1)//2) and is_47_smooth((x_k+1)//2):
                    ans.add((x_k-1)//2)
    else:
        base = primes[depth]
        for include in [False, True]: # include prime in the product or not
            next_product = product
            if include:
                next_product = next_product * base
            dfs(depth+1, target_depth, next_product, ans, primes)

if __name__ == "__main__":
    ans = set()
    dfs(0, len(primes), 1, ans, primes)
    print(sum(ans))
