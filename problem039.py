# Question: https://projecteuler.net/problem=39

# a + b + c = p
#    a^2 + b^2 = c^2
# -> a^2 + b^2 - (p-a-b)^2 = 0
# -> a^2 + b^2 - (p^2 + a^2 + b^2 - 2pa - 2pb + 2ab) = 0
# -> -p^2 + 2pa + 2pb - 2ab = 0
# -> -p^2 + 2a(p-b) + 2pb = 0
# -> a = (p^2-2pb)/2(p-b) = p(p-2b)/2(p-b)
# -> p(p-2b) is even
# if p is odd -> p-2b is odd -> p(p-2b) is odd (contradiction)
# -> p must be even
# Let p = 2k -> a = 2k(2k-2b)/2(2k-b) = 2k(k-b)/(2k-b)
# p <= 1000 -> k <= 500

count = {}

for k in range(500+1):
    for b in range(1, k):
        if 2*k*(k-b) % (2*k-b) == 0:
            if not 2*k in count:
                count[2*k] = 0
            count[2*k] = count[2*k] + 1

max_p = 0
max_count = 0
for p, c in count.items():
    if c > max_count:
        max_p = p
        max_count = c

print(max_p)