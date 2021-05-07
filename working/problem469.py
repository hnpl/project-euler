N = 600

if __name__ == "__main__":
    ans = 0

    En = {0: 0}

    for n in range(1, N+1):
        exp = 0
        for i in range(1, n+1):
            if n-i-1 >= 0:
                exp += En[n-i-1]
            if i-2 >= 0:
                exp += En[i-2]
            exp += 1.0
        En[n] = exp/n

    ans = (N - (En[N-3] + 1)) / N

    print(ans)
