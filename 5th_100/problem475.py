from sage.all import *

N = 600
M = N // 3
MOD = 10**9+7
BIT_WIDTH = 8

def get_mask(n):
    return 2**n - 1

cached_factorials = []
cached_inversed_factorials = []
cached_binomial = {}

cache_hit_count = 0
cache_count = 0

def fast_binomial(n, k, mod):
    key = f"{n}{k}"
    if not (n, k) in cached_binomial:
        cached_binomial[(n, k)] = (((cached_factorials[n] * cached_inversed_factorials[k]) % mod) * cached_inversed_factorials[n-k]) % mod
    else:
        global cache_hit_count
        cache_hit_count += 1
    global cache_count
    cache_count += 1
    return cached_binomial[(n, k)]

class State:
    def __init__(self, n_3s, n_2s, n_1s):
        self.n_3s = n_3s
        self.n_2s = n_2s
        self.n_1s = n_1s
        self.hash = State.encode(n_3s, n_2s, n_1s)
    def init_from_hash(hash):
        n_3s, n_2s, n_1s = State.decode(hash)
        new_state = State(n_3s, n_2s, n_1s)
        return new_state
    def get_hash(self):
        return self.hash
    def encode(n_3s, n_2s, n_1s):
        h = 0
        msb = 0
        h |= (n_3s << msb)
        msb += BIT_WIDTH
        h |= (n_2s << msb)
        msb += BIT_WIDTH
        h |= (n_1s << msb)
        return h
    def decode(h):
        n_3s = h & get_mask(BIT_WIDTH)
        h >>= BIT_WIDTH
        n_2s = h & get_mask(BIT_WIDTH)
        h >>= BIT_WIDTH
        n_1s = h & get_mask(BIT_WIDTH)
        return n_3s, n_2s, n_1s
    def to_string(self):
        return f"({self.n_3s}, {self.n_2s}, {self.n_1s})"
    def get_next_states(self, n_tripplets):
        n_3s, n_2s, n_1s = State.decode(self.hash)
        next_states = {}
        for count_3 in range(5):
            for count_2 in range(5-count_3):
                count_1 = 4 - count_3 - count_2
                assert(count_1 >= 0)
                new_n_3s = n_3s - count_3
                new_n_2s = n_2s + count_3 - count_2
                new_n_1s = n_1s + count_2 - count_1
                #print(new_n_3s, new_n_2s, new_n_1s)
                if new_n_3s < 0 or new_n_2s < 0 or new_n_1s < 0 or count_2 > n_2s or count_1 > n_1s:
                    continue
                next_state = State(new_n_3s, new_n_2s, new_n_1s)
                #print(self.to_string(), "->", next_state.to_string(), "=", count_3, count_2, count_1)
                next_state_hash = next_state.get_hash()
                if not next_state_hash in next_states:
                    next_states[next_state_hash] = 0
                #next_states[next_state_hash] += binomial(n_2s, count_2) * binomial(n_1s, count_1) * factorial(count_2) * factorial(count_1) * binomial(4, count_3) * binomial(4-count_3, count_2)
                c = fast_binomial(n_2s, count_2, MOD) * fast_binomial(n_1s, count_1, MOD)
                c %= MOD
                c *= cached_factorials[count_2] * cached_factorials[count_1] * binomial(4, count_3) * binomial(4-count_3, count_2)
                c %= MOD
                next_states[next_state_hash] += c
                next_states[next_state_hash] %= MOD
        return next_states

def warm_cache(m):
    cached_factorials.append(1)
    cached_inversed_factorials.append(multiplicative_order(Mod(1, MOD)))
    for i in range(1, m+1):
        cached_factorials.append((cached_factorials[-1] * i) % MOD)
        cached_inversed_factorials.append(inverse_mod(cached_factorials[-1], MOD))

if __name__ == "__main__":
    N = 600
    warm_cache(N // 3)

    curr = {}
    prev = {}

    initial_state = State(n_3s = N//3, n_2s = 0, n_1s = 0)
    initial_state_hash = initial_state.get_hash()
    curr[initial_state_hash] = 1

    for i in range(N//4):
        curr, prev = {}, curr
        for prev_state_hash, prev_state_count in prev.items():
            prev_state = State.init_from_hash(prev_state_hash)
            for next_state_hash, next_state_count in prev_state.get_next_states(i).items():
                if not next_state_hash in curr:
                    curr[next_state_hash] = 0
                curr[next_state_hash] += (prev_state_count * next_state_count) % MOD
                curr[next_state_hash] %= MOD
                next_state = State.init_from_hash(next_state_hash)
                #print(prev_state.to_string(), "->", next_state.to_string(), "=", curr[next_state_hash])
        print(i, len(curr), cache_hit_count, cache_count)
    ans = 0
    for state, count in curr.items():
        ans += count
    print(ans % MOD)
