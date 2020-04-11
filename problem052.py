# Question: https://projecteuler.net/problem=52

def check(num):
    digits = {digit for digit in str(num)}
    for i in range(2, 7):
        s = str(i * num)
        for digit in s:
            if not digit in digits:
                return False
    return True

num_digits = 1
stop = False

while not stop:
    num_digits = num_digits + 1
    for x in range(10**num_digits, (10**(num_digits+1) // 6)):
        if check(x):
            stop = True
            break

print(x)
