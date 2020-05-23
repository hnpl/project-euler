# Question: https://projecteuler.net/problem=42

N = 1000

T = {n*(n+1)//2 for n in range(N)}

with open("inputs/p042_words.txt", "r") as f:
    words = f.readline().strip()[1:-1].split("\",\"")

count = 0
for word in words:
    word_score = sum([ord(c) - ord('A') + 1 for c in word])
    if word_score in T:
        count = count + 1
print(count)
