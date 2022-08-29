from sage.all import *

N = 10**6
K = 10**12
MOD = 10**9 + 7
def brute_force(n):
    n_str = str(n)
    n_digits = len(n_str)
    ans = 0
    for length in range(1, n_digits+1):
        for start in range(n_digits - length + 1):
            d = int(n_str[start:start+length], 10)
            ans += d
    return ans

#def S(n):
#    ans = 0
#
#    n_str = str(n)
#    n_digits = len(n_str)
#
#    coeff = [0] * n_digits
#    coeff[0] = 1
#    for i in range(1, n_digits):
#        coeff[i] = (pow(10, i, MOD) + coeff[i-1]) % MOD
#
#    for inversed_place_value, digit_str in enumerate(n_str):
#        digit = int(digit_str, 10)
#        place_value = n_digits - inversed_place_value - 1
#        n_appearances_per_place_value = inversed_place_value + 1
#        print(digit, coeff[place_value], n_appearances_per_place_value)
#        ans += digit * coeff[place_value] * n_appearances_per_place_value
#        ans %= MOD
#
#    return ans

def rho(k):
    return (10**(k) - 1)//9

def S(n, n_periods):
    R = IntegerModRing(MOD)
    ans = R(0)

    n_str = str(n)
    period_length = len(n_str)
    n_periods = n_periods
    n_digits = n_periods * period_length

    for inversed_place_value, digit_str in enumerate(n_str):
        if inversed_place_value % 10000 == 0:
            print(inversed_place_value)
        digit = int(digit_str, 10)
        offset = inversed_place_value + 1
        coeff_sum = 0
        #for period_idx in range(n_periods):
            #coeff = 1
            #coeff *= rho(n_digits + 1 - offset - period_idx * period_length)
            #coeff *= offset + period_length * period_idx
            #coeff_sum += coeff
            #coeff = (10**(n_digits + 1 - offset - period_idx * period_length)-1)*(offset+period_idx * period_length)
            #coeff_sum += coeff
        #print("expected coeff", coeff_sum)
        #ans += digit * coeff_sum // 9
        # Since multiplicative_order(10, 10**9+7) = 10**9+6, then L, N, a in 10**(L+1-a) (mod 10**9+7) must be of the ModRing(10**9+6)! Putting L, N, a to IntegerModRing(10**9+7) would result in an incorrect result.
        L = period_length
        assert(isinstance(L, int))
        N = n_periods
        assert(isinstance(N, int))
        a = offset
        assert(isinstance(a, int))
        part_1 = a*((R(10)**(L+1-a))*(R(10)**(L*N)-1) // (R(10)**L-1) - N)
        part_2 = L * (2*(R(10)**(L+1-a))*(R(10)**(L*N)-1) + (1-R(10)**L)*(N*(2*(R(10)**(L+1-a))-R(10)**L+1)) - (N**2)*((R(10)**L-1)**2)) // (2*((R(10)**L-1)**2))
        coeff = part_1 + part_2
        ans += digit * coeff // 9
    return ans

def C(n):
    p = primes_first_n(n)
    p = list(map(str, p))
    ans = "".join(p)
    return ans

if __name__ == "__main__":
    print(S(C(N), K))
