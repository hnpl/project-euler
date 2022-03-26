# Problem: https://projecteuler.net/problem=776
#
# The idea is similar to that of problem 759

N = 1234567890123456789

class CacheSubUnit:
    def __init__(self, element_sum = 0, element_count = 0):
        self.element_sum = element_sum
        self.element_count = element_count

    def init_from_cache_sub_unit(other):
        return CacheSubUnit(other.element_sum, other.element_count)

    def merge(self, other):
        self.element_sum += other.element_sum
        self.element_count += other.element_count

    def get_element_sum(self):
        return self.element_sum

    def get_offset_cache_sub_unit(self, offset):
        return CacheSubUnit(element_sum = self.element_sum + offset * self.element_count,
                            element_count = self.element_count)

    def get_current_and_offset_cache_sub_unit(self, offset):
        return CacheSubUnit(element_sum = self.element_sum + (self.element_sum + offset * self.element_count),
                            element_count = 2 * self.element_count)

    def to_string(self):
        return f"sum={self.element_sum}; count={self.element_count}"

class CacheUnit:
    def __init__(self):
        self.sub_units = {}

    def init_from_cache_unit(other):
        new_cache_unit = CacheUnit()
        for digit_sum, sub_unit in self.sub_units.items():
            new_cache_unit[n_digits] = CacheSubUnit.init_from_cache_sub_unit(sub_unit)
        return new_cache_unit

    def get_digit_sum_division(self):
        ans = 0.0
        for digit_sum, sub_unit in self.sub_units.items():
            if digit_sum == 0:
                continue
            ans += sub_unit.element_sum / digit_sum
        return ans

    def merge(self, other):
        for others_digit_sum, others_sub_unit in other.sub_units.items():
            if not others_digit_sum in self.sub_units:
                self.sub_units[others_digit_sum] = CacheSubUnit(element_sum = 0, element_count = 0)
            self.sub_units[others_digit_sum].merge(others_sub_unit)

    def get_digit_sum(n):
        ans = 0
        while n > 0:
            ans += n % 10
            n //= 10
        return ans

    def get_offset_cache_unit(self, offset):
        new_cache_unit = CacheUnit()
        offset_digit_sum = CacheUnit.get_digit_sum(offset)
        for digit_sum, sub_unit in self.sub_units.items():
            new_digit_sum = digit_sum + offset_digit_sum
            new_sub_unit = sub_unit.get_offset_cache_sub_unit(offset)
            new_cache_unit.sub_units[new_digit_sum] = new_sub_unit
        return new_cache_unit

    def to_string(self):
        return "\n".join([f"{digit_sum}: " + sub_unit.to_string() for digit_sum, sub_unit in self.sub_units.items()])

def F(n):
    ans = 0

    cache = {}
    cache[0] = CacheUnit()
    cache[0].sub_units = {0: CacheSubUnit(element_sum = 0, element_count = 1)}
    cache[1] = CacheUnit()
    cache[1].sub_units = {digit: CacheSubUnit(element_sum = digit, element_count = 1) for digit in range(10)}

    # Get the number of digits of n
    n_digits = 0
    w = n
    while w > 0:
        n_digits += 1
        w //= 10

    # Building the cache
    for k in range(2, n_digits+1):
        cache[k] = CacheUnit()
        for next_digit in range(10): # prepend next_digit to every current number
            cache[k].merge(cache[k-1].get_offset_cache_unit(offset=next_digit * (10**(k-1))))

    # left to right pass
    ans_unit = CacheUnit()
    digits = list(map(int, reversed(str(n))))
    offset = 0
    for digit_index in range(len(digits)-1, -1, -1):
        max_digit = digits[digit_index]
        for digit in range(max_digit):
            ans_unit.merge(cache[digit_index].get_offset_cache_unit(offset = offset + digit * (10**(digit_index))))
        offset += max_digit * (10**(digit_index))
    ans = ans_unit.get_digit_sum_division()
    ans += n / CacheUnit.get_digit_sum(n)

    return ans

if __name__ == "__main__":
    ans = F(N)
    print(f"{ans:.12e}")
