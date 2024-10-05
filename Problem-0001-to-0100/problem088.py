# Problem: https://projecteuler.net/problem=88

"""
    . The upperbound for the sum/product is 2 * 12000.
      Why?
        For every number k, one of product/sum combinations is 1 + 1 + ... + 1 + 2 + k = 1 * 1 * ... * 1 * 2 * k
                                                               ^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^
                                                               (k-2) 1's
    . Generate all multiplicative partitions (https://en.wikipedia.org/wiki/Multiplicative_partition) for each n for 2 <= n <= 2*12000.
        . i.e. generate all possible ways to factor n (without factors of 1)
        . For each possible ways to factor n, since adding 1's do not affect the product, we can add (product - current_sum) ones until product = sum = n.
            .  We count the number of multiplicands after ones were added, supposely k, and then record the best sum/product for k multiplicands or k addends.
"""

N = 12000
product_upperbound = N * 2

def partition_list_from_bit_mask(l, bit_mask):
    list1 = []
    list2 = []
    index = 0
    
    n = len(l)
    for i in range(n):
        bit = bit_mask % 2
        if bit == 0:
            list1.append(l[i])
        else:
            list2.append(l[i])
        bit_mask = bit_mask // 2

    return list1, list2

def hash_list(l):
    r = sorted(l)
    h = 0
    for i in r:
        h = h*N+i
    return h

def partitions_helper(l, path):
    if len(l) == 0:
        yield path
    n = len(l)
    seen = set()
    for bit_mask in range(1, 2**n):
        list1, list2 = partition_list_from_bit_mask(l, bit_mask)
        if hash_list(list1) in seen:
            continue
        seen.add(hash_list(list1))
        path.append(list2) # list2 must be non-empty
        yield from partitions_helper(list1, path)
        del path[-1]

def partitions(l):
    yield from partitions_helper(l, [])

def load_primes(up_to):
    primes = []
    with open('../inputs/primes_1e6.txt', 'r') as f:
        for line in f:
            prime = int(line.strip())
            if prime >= up_to:
                break
            primes.append(prime)
    return primes

def factorize(n, primes):
    factors = []
    for prime in primes:
        while n % prime == 0:
            factors.append(prime)
            n = n // prime
        if n == 1:
            break
    return factors

def multiplicative_partitions(l):
    for partition in partitions(l):
        p = []
        for sub_partition in partition:
            n = 1
            for element in sub_partition:
                n = n * element
            p.append(n)
        yield p

if __name__ == "__main__":
    primes = load_primes(up_to = product_upperbound)
    best_for = [2**32] * (N+1)
    
    best_for[0] = 0
    best_for[1] = 0

    for product in range(2, product_upperbound+1):
        factors = factorize(product, primes)
        for partition in multiplicative_partitions(factors):
            k = len(partition) + product - sum(partition)
            if k <= N:
                best_for[k] = min(best_for[k], product)
    ans = sum(set(best_for))
    print(ans)
