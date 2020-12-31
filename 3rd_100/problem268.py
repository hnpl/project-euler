# Question: https://projecteuler.net/problem=268

N = 10**16
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

class Result:
    def __init__(self):
        self.ans = 0

def get_coeff(n):
    if n % 2 == 1:
        return -(n**3+11*n)//6 + n**2 + 1
    else:
        return (n**3+11*n)//6 - n**2 - 1

def dfs(depth, n_on, curr_val, max_depth, primes, result):
    if depth == max_depth:
        return
    next_prime = primes[depth]
    dfs(depth+1, n_on, curr_val, max_depth, primes, result)
    next_val = curr_val * next_prime
    if next_val < N:
        result.ans += N // next_val * get_coeff(n_on+1)
        dfs(depth+1, n_on+1, next_val, max_depth, primes, result)


result = Result()
dfs(0, 0, 1, len(primes), primes, result)

print(result.ans)
