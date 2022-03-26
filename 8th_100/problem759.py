# Problem: https://projecteuler.net/problem=759
#
# Hint: http://oeis.org/A245788
# TODO: explain this :p

N = 10**16
MOD = 10**9 + 7

class CacheSubUnit: # Store all the info to calculate sum(f(j)^2) for j in [2^n, 2^(n+1)-1]
                    # so that we perform next calculation, e.g. sum(f(j)^2) for j in [2^(n+1), 2^(n+2)-1]
    def __init__(self, element_sum = 0, element_squared_sum = 0, element_count = 0):
        self.element_sum = element_sum
        self.element_squared_sum = element_squared_sum
        self.element_count = element_count
    
    def init_from_cache_sub_unit(other):
        new_cache_sub_unit = CacheSubUnit(element_sum = other.element_sum,
                                          element_squared_sum = other.element_squared_sum,
                                          element_count = other.element_count)
        return new_cache_sub_unit

    def get_squared_sum(self):
        return self.element_squared_sum

    def get_offset_squared_sum(self, offset): # get squared sum where each element is added by an "offset" amount
        return self.element_squared_sum + 2 * offset * self.element_sum + (offset**2) * self.element_count

    def get_offset_cache_sub_unit(self, offset):
        return CacheSubUnit(element_sum = self.element_sum + offset * self.element_count,
                            element_squared_sum = self.element_squared_sum + 2 * offset * self.element_sum + (offset**2) * self.element_count,
                            element_count = self.element_count)

    def sum_current_and_offset_cache_sub_units(self, offset):
        return CacheSubUnit(element_sum = self.element_sum + (self.element_sum + offset * self.element_count),
                            element_squared_sum = self.element_squared_sum + (self.element_squared_sum + 2 * offset * self.element_sum + (offset**2) * self.element_count),
                            element_count = self.element_count * 2)

    def to_string(self):
        return f"sum={self.element_sum} squared_sum={self.element_squared_sum} count={self.element_count}"

class CacheUnit:
    def __init__(self):
        self.sub_units = {}

    def init_from_cache_unit(other):
        new_cache_unit = CacheUnit()
        for n_bits, sub_unit in other.sub_units.items():
            new_cache_unit.sub_units[n_bits] = CacheSubUnit.init_from_cache_sub_unit(sub_unit)
        return new_cache_unit

    def get_squared_sum(self):
        ans = 0
        for n_bits, phi in self.sub_units.items():
            ans += ((n_bits) ** 2) * phi.get_squared_sum()
        return ans

    def get_offset_squared_sum(self, offset):
        n_1_bits_in_offset = 0
        w = offset
        while w > 0:
            if w % 2 == 1:
                n_1_bits_in_offset += 1
            w //= 2
 
        ans = 0
        for n_bits, sub_unit in self.sub_units.items():
            new_n_bits = n_bits + n_1_bits_in_offset
            cache_sub_unit = CacheSubUnit.init_from_cache_sub_unit(sub_unit)
            cache_sub_unit = cache_sub_unit.get_offset_cache_sub_unit(offset)
            ans += (new_n_bits**2) * cache_sub_unit.get_squared_sum()
        return ans

    def sum_current_and_offset_cache_units(self, offset):
        n_1_bits_in_offset = 0
        w = offset
        while w > 0:
            if w % 2 == 1:
                n_1_bits_in_offset += 1
            w //= 2

        new_cache_unit = CacheUnit.init_from_cache_unit(self)
        for n_bits, sub_unit in self.sub_units.items():
            new_n_bits = n_bits + n_1_bits_in_offset
            cache_sub_unit = CacheSubUnit.init_from_cache_sub_unit(self.sub_units[n_bits])
            cache_sub_unit = cache_sub_unit.get_offset_cache_sub_unit(offset)
            if new_n_bits in new_cache_unit.sub_units:
                cache_sub_unit.element_sum += new_cache_unit.sub_units[new_n_bits].element_sum
                cache_sub_unit.element_squared_sum += new_cache_unit.sub_units[new_n_bits].element_squared_sum
                cache_sub_unit.element_count += new_cache_unit.sub_units[new_n_bits].element_count
            new_cache_unit.sub_units[new_n_bits] = cache_sub_unit

        return new_cache_unit

    def to_string(self):
        s = [f"{n_bits}: " + sub_unit.to_string() for n_bits, sub_unit in self.sub_units.items()]
        return "\n".join(s)

def S(n):
    cache = {}
    cache[1] = CacheUnit()
    cache[1].sub_units[0] = CacheSubUnit(element_sum = 0, element_squared_sum = 0, element_count = 1)
    cache[1].sub_units[1] = CacheSubUnit(element_sum = 1, element_squared_sum = 1, element_count = 1)

    # building the cache
    n_bits = 0
    # - count how many bits n has
    w = n
    while w > 0:
        n_bits += 1
        w >>= 1
    # - building the cache up to that amount of bits
    for k in range(2, n_bits+1):
        offset = 2**(k-1)
        cache[k] = cache[k-1].sum_current_and_offset_cache_units(offset)
    # calculating S
    ans = 0

    ans += cache[n_bits-1].get_squared_sum()

    n_bit_pattern = []
    w = n
    while w > 0:
        n_bit_pattern.append(w%2)
        w //= 2
    offset = 2**(n_bits-1)
    for bit_index in range(len(n_bit_pattern)-2, 0, -1):
        bit = n_bit_pattern[bit_index]
        if bit == 0:
            continue
        ans += cache[bit_index].get_offset_squared_sum(offset)
        offset += (2**(bit_index)) * bit
    ans += (n**2) * (sum(n_bit_pattern))**2
    return ans

if __name__ == "__main__":
    print(S(N)%MOD)
