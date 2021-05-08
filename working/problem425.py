from sage.all import *
import queue
import itertools

N = 10**7
max_n_digits = int(log(N, 10)) + 1

primes = prime_range(N)
primes_set = set(primes)

def get_n_digits(n):
    count = 0
    while n > 0:
        n //= 10
        count += 1
    return count

def get_digits(n):
    digits = []
    while n > 0:
        digits.append(n%10)
        n //= 10
    return digits

def generate_case_1(n, primes_set):
    n_digits = get_n_digits(n)
    if n_digits >= max_n_digits:
        return []
    coeff = 10 ** n_digits
    for first_digit in range(1, 10):
        next_n = n + first_digit * coeff
        if next_n in primes_set:
            yield next_n

def generate_case_2(n, primes_set):
    digits = get_digits(n)
    coeff = 1
    for digit_pos, digit in enumerate(digits):
        start_digit = 0
        if digit_pos == 0:
            start_digit = 1
        for next_digit in range(start_digit, 10):
            next_n = n - (digit * coeff) + (next_digit * coeff)
            if next_n in primes_set:
                yield next_n
        coeff *= 10

if __name__ == "__main__":
    ans = 0

    q = queue.PriorityQueue()
    prev_max = {2: 2}
    seeds = [3,5,7]
    for seed in seeds:
        q.put(seed)
        prev_max[seed] = seed

    while not q.empty():
        p = q.get()
        for next_p in itertools.chain(generate_case_1(p, primes_set), generate_case_2(p, primes_set)):
            if not next_p in prev_max:
                q.put(next_p)
                prev_max[next_p] = max(prev_max[p], next_p)

    ans = sum(primes)
    for prime, m in prev_max.items():
        if m <= prime:
            ans -= prime

    print(ans)
