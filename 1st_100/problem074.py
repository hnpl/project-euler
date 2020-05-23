# Question: https://projecteuler.net/problem=74

N = 10**6 - 1

class Result:
    def __init__(self):
        self.sum_digits = {}
        self.chain_length = {}
        self.f = {'0': 1}
        for i in range(1, 10):
            self.f[str(i)] = self.f[str(i-1)] * i

    def calculate_sum_digits(self, n):
        return sum([self.f[digit] for digit in str(n)])

    def forward(self, n):
        if not n in self.sum_digits:
            self.sum_digits[n] = self.calculate_sum_digits(n)
            r = self.forward(self.sum_digits[n])
            return r + [n]
        else:
            return [n]
    def backward(self, queue):
        if queue[0] in queue[1:]:
            queue = queue[1:]
        if not queue[0] in self.chain_length:
            self.chain_length[queue[0]] = 1
        curr = self.chain_length[queue[0]]
        for n in queue[1:]:
            curr = curr + 1
            self.chain_length[n] = curr

    def add(self, n):
        if n in self.sum_digits:
            return
        queue = self.forward(n)
        self.backward(queue)

        
r = Result()

for i in range(1, N+1):
    r.add(i)

count = 0

for k, v in r.chain_length.items():
    if v == 60:
        count = count + 1

print(count)
