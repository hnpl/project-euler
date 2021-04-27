from sage.all import *
import queue

N = 10**6
M = 200
primes = prime_range(N)

class Node:

    def __init__(self, p_i, q_i):
        self.p_i = p_i
        self.q_i = q_i
        self.prod = (primes[p_i]**2) * (primes[q_i]**3)

    def get_next_node(self):
        p_i = self.p_i
        q_i = self.q_i + 1
        if p_i == q_i:
            q_i += 1
        return Node(p_i, q_i)
    
    def __gt__(self, rhs):
        return self.prod > rhs.prod
    
    def __eq__(self, rhs):
        return self.prod == rhs.prod
    
    def __repr__(self):
        return "Node({}^2,{}^3,{})".format(primes[self.p_i], primes[self.q_i], self.prod)

def is_prime_proof(n): # n is prime-proof when replacing one of its digit does not turn n into a prime number
    n = list(str(n))
    for replacement_position in range(len(n)):
        new_n = n[:]
        digit_set = None
        if replacement_position == 0:
            digit_set = range(1, 10)
        else:
            digit_set = range(0, 10)
        for new_digit in digit_set:
            if new_digit == n[replacement_position]:
                continue
            new_n[replacement_position] = str(new_digit)
            if is_prime(int("".join(new_n))):
                return False
    return True

def has_200(n):
    n = str(n)
    if n.find("200") >= 0:
        return True
    return False

if __name__ == "__main__":
    ans = 0
    n = len(primes)
    _ = """
    curr_p_i = {prime: 0 for prime in primes}
    curr_p_i[2] = 1 # (2**2) * (2**3) is not valid
    next_prod = {prime: (prime**2) * (primes[curr_p_i[prime]]**3) for prime in primes}
    for _ in range(10000):
        min_prod_prime = 2
        min_prod = next_prod[2]
        for prime in primes:
            if min_prod > next_prod[prime]:
                min_prod = next_prod[prime]
                min_prod_prime = prime
    """
    q = queue.PriorityQueue()
    q.put(Node(0, 1)) # Node(0,0) is invalid
    for p_i in range(1, n):
        q.put(Node(p_i, 0))
    count = 0
    node = None
    while count < M:
        node = q.get()
        if has_200(node.prod) and is_prime_proof(node.prod):
            count += 1
        q.put(node.get_next_node())
    ans = node.prod
    print(ans)
