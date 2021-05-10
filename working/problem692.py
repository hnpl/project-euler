
# https://en.wikipedia.org/wiki/Fibonacci_nim

N = 23416728348467685

def generate_fibonacci_seq(limit):
    seq = []
    a, b = 1, 1
    while b <= limit:
        a, b = b, a + b
        seq.append(a)
    return seq

if __name__ == "__main__":
    F = generate_fibonacci_seq(N)
    H = [1, 3, 6]
    for i in range(3, len(F)):
        H.append(H[-1] + H[i-2] - F[i-2] + F[i])
    ans = H[-1]
    print(ans)
