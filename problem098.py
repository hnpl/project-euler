# https://projecteuler.net/problem=98

from collections import Counter, defaultdict
from math import sqrt

hashes = {}
max_n_digits = 0

anagram = set()

def getHash(s):
    freq = Counter(s)
    keys = sorted(freq.keys())
    h = ["{}:{}".format(k,freq[k]) for k in keys]
    return ",".join(h)

with open('inputs/p098_words.txt', 'r') as f:
    words = f.readlines()[0].strip()[1:-1].split("\",\"")
    for word in words:
        h = getHash(word)
        if not h in hashes:
            hashes[h] = []
        hashes[h].append(word)

to_remove = []

n_max_digits = 0

anagram_map = {}

def new_hash(word):
    word_map = {}
    n = 0
    h = ''
    for i, c in enumerate(word):
        if not c in word_map:
            word_map[c] = str(n)
            n = n + 1
        h = h + word_map[c]
    return h, word_map

def rearrangement(word, word_map):
    h = ''
    for c in word:
        h = h + word_map[c]
    return h

# rearrange('abcde', '34521') = 'cdeba'
def rearrange(word, pattern):
    h = ''
    for i in pattern:
        h = h + word[int(i)]
    return h


for h, words in hashes.items():
    if not len(words) > 1:
        to_remove.append(h)
    else:
        n_max_digits = max(n_max_digits, len(words[0]))
        for i_word1 in range(len(words)-1):
            for i_word2 in range(i_word1, len(words)):
                word1 = words[i_word1]
                word2 = words[i_word2]
                word1_hash, word1_map = new_hash(word1)
                if not word1_hash in anagram_map:
                    anagram_map[word1_hash] = set()
                anagram_map[word1_hash].add(rearrangement(word2, word1_map))
                word2_hash, word2_map = new_hash(word2)
                if not word2_hash in anagram_map:
                    anagram_map[word2_hash] = set()
                anagram_map[word2_hash].add(rearrangement(word1, word2_map))

sqt = sqrt(10**(n_max_digits+1)-1)+1
squares = [str(i*i) for i in range(int(sqt), -1, -1)]
square_set = set(squares)

found = False

for square in squares:
    square_hash, _ = new_hash(square)
    if not square_hash in anagram_map:
        continue
    for pattern in anagram_map[square_hash]:
        r = rearrange(square, pattern)
        if r in square_set and not r == square:
            found = True
            break
    if found:
        break

print(square)
