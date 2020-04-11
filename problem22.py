# Question: https://projecteuler.net/problem=22

with open("inputs/p022_names.txt", "r") as f:
    names = f.readline().strip()[1:-1].split("\",\"")
names = sorted(names)
score = 0
for i, name in enumerate(names):
    s = 0
    for c in name:
        s = s + ord(c) - ord('A') + 1
    score = score + s * (i+1)
print(score)
