# Question: https://projecteuler.net/problem=4

# An integer of the form abccba is divisible by 11 as abccba = 11 * (9091*a + 910*b + 100*c)
# Let abccba = 11 * X * Y; where X in [10, 90] and Y in [100, 999]

result = -1
Xstop = False

def isPalindrome(num):
    s = str(num)
    return s[0] == s[-1] and s[1] == s[-2] and s[2] == s[-3]

for X in range(90, 9, -1):
    X = X * 11
    if int(999999//X) < 100:
        break
    for Y in range(min(int(999999//X), 999), max(int(100000//X), 100), -1):
        if isPalindrome(X*Y):
            result = max(result, X * Y)
            break

print(result) 