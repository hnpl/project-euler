# https://projecteuler.net/problem=14

d = {1: 1}

def forward(num):
    chain = [num]
    while not num in d:
        if num % 2 == 0:
            num = num // 2
        else:
            num = 3 * num + 1
        chain.append(num)
    return chain, len(chain) + d[chain[-1]] - 1

def backward(chain, d):
    if chain:
        p = d[chain[-1]]
        for i, num in enumerate(reversed(chain)):
            d[num] = p + i

longest_chain_length = 1
starting_number = 1

for i in range(2, int(1e6)+1):
    chain, chain_length = forward(i)
    assert(chain_length > 0)
    if chain_length > longest_chain_length:
        longest_chain_length = chain_length
        starting_number = i
    backward(chain, d)

print(starting_number)
