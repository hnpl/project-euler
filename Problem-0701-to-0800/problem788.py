# Problem: https://projecteuler.net/problem=788

from scipy.special import comb as C

N = 2022
MOD = 1000000007

# prototype
def count(n):
    ans = 0
    # Case 1: the first digit (from the left) is not a dominating digit
    #   1 digit   | k digits          | n-k-1 digits
    # [ 1st digit | dominating_digits | other_digits ]
    #   alpha       beta                gamma
    # if beta={1...9} then gamma={0...9}\beta, alpha={1...9}\beta   (a\b = {x in a AND x not in b})
    # if beta={0} then gamma={1...9}, alpha={1...9}
    for k in range(n//2+1, n+1): # k -> number of dominating digits
        if n-1 < k:
            continue
        ans += C(n-1, k, exact=True) * 9 * (9**(n-k-1)) * 8
        ans %= MOD
        ans += C(n-1, k, exact=True) * 9 * (9**(n-k-1))
        ans %= MOD
    # Case 2: the first digit (from the left) is a dominating digit
    #   k digits            n-k digits
    # [ dominating_digits | other_digits ]
    #   beta                gamma
    # beta in {1...9}, gamma in {0...9}\beta
    for k in range(n//2+1, n+1):
        ans += C(n-1, k-1, exact=True) * 9 * (9**(n-k))
        ans %= MOD
    return ans

def count2(n):
    ans = 0
    prev_alpha = C(n-1, n//2, exact=True)
    k = n//2
    for k in range(n//2+1, n):
        alpha = prev_alpha * (n-k) // k
        ans += prev_alpha * 9 * (9**(n-k)) + alpha * 9 * (9**(n-k-1)) * 9
        prev_alpha = alpha
    alpha = 1
    k = n
    ans += alpha * 9 * (9**(n-k))
    return ans

if __name__ == "__main__":
    ans = 0

    ans += 9 # 1-digit numbers
    ans += 9 # 2-digit numbers
    for i in range(3, N+1):
        ans += count2(i)

    print(ans % MOD)
