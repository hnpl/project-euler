# Question: https://projecteuler.net/problem=36

N = 10**6

def is_palindromic(num, base):
	if base == 10:
		s = str(num)
	elif base == 2:
		s = "{0:b}".format(num)
	N = len(s)
	for i in range(N//2):
		if not s[i] == s[N-i-1]:
			return False
	return True

sum = 0

for i in range(N):
	if is_palindromic(i, base = 10) and is_palindromic(i, base = 2):
		sum = sum + i

print(sum)