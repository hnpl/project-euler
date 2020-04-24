# Question: https://projecteuler.net/problem=69

# So we want to maximize n / phi(n) = n / (n * product_{p|n, p \in primes}(1-1/p)) = 1 / [product_{p|n, p \in primes}(1-1/p)]
# In order words, we want to minimize [product_{p|n, p \in primes}(1 - 1/p)]
# Since (1 - 1/p) < 1 for all primes >= 2 -> we want as many primes as possible

N = 10**6
result = 1
with open('inputs/primes_1e6.txt', 'r') as f:
    for line in f:
        prime = int(line.strip())
        if result * prime > N:
            break
        result = result * prime
print(result)
