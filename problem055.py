# Question: https://projecteuler.net/problem=55

def reverse(n):
    return int(''.join(reversed(str(n))))

def is_a_palindrome(n):
    return list(str(n)) == list(str(reverse(n)))

def check(n):
    for i in range(50):
        n = n + reverse(n)
        if is_a_palindrome(n):
            return True
    return False

count = 0

for i in range(1, 10000):
    if not check(i):
        count = count + 1

print(count)  
