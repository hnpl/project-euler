# Question: https://projecteuler.net/problem=50

primes = prime_range(1e6)
N = len(primes) # about 80k
M = 1e6
prime_set = set(primes)

# for each starting point, we need to find the longest sequence in O(N)
# if we know one sequence, we don't need to consider sorter ones.

longest_seq_length = 21
longest_seq_prime = 953

for i, prime in enumerate(primes):
    if i + longest_seq_length >= N:
        break
    s = sum(primes[i:i+longest_seq_length]) # can do a bit better here
    for j in range(i+longest_seq_length, N):
        s = s + primes[j]
        if s > M:
            break
        if s in primes:
            longest_seq_length = j - i + 1
            longest_seq_prime = s

print(longest_seq_prime)
