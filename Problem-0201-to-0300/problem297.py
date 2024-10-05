# Problem: https://projecteuler.net/problem=297

"""
    . Let f(i) denote the i^th Fibonacci number.
    . Note that,
        f(1) + f(3) + ... + f(2k-1) = f(2k)-1 (*)
        Why?
            f(2k) = f(2k-1) + f(2k-2)
                  = f(2k-1) + f(2k-3) + f(2k-4)
                  = ...
                  = f(2k-1) + f(2k-3) + ... + f(1) + f(0)
                  = left_hand_side + 1
    . Similarly, f(2) + f(4) + ... + f(2k) = f(2k+1) - 1 (**)
    . We can use a greedy algorithm to find the Zeckendorf's representation of a number. (1)
    . According to https://mathworld.wolfram.com/ZeckendorfRepresentation.html, each positive integer has a unique Zeckendorf's representation. (2)
    . From (*), (**), (1) and (2), we can conclude that, with the first k Fibonacci numbers, we can find the Zeckendorf's representation of numbers from 1 to f(k+1) - 1.

    . Let DP[i] denote the number of Fibonacci numbers needed to represent numbers from 1 to f(i+1)-1.
    . Note that,
        If we know DP[i], we can figure out the number of Fibonacci numbers needed to represent numbers from f(i+2)+1 to f(i+3)-1.
        Why? For each of number k from 1 to f(i+1)-1, we have k + f(i+2) is in the range f(i+2)+1 to f(i+3)-1.
        The number of Fibonacci numbers needed to represent numbers from f(i+2)+1 to f(i+3)-1 is DP[i] + f(i+1) - 1.
    . Also, if we know DP[i+1], we know the number of Fibonacci numbers needed to represent numbers from f(i+3)+1 to f(i+4)-1.
        The number of Fibonacci numbers needed to represent numbers from f(i+3)+1 to f(i+4)-1 is DP[i+1] + f(i+2) - 1.
    . So, DP[i+2] = (# numbers from 1 to f(i+2)) + (# numbers from f(i+2) to f(i+3)-1) = DP[i+1] + 1 + DP[i] + f(i+1)-1 = DP[i+1]+DP[i]+f(i+1)

    . Note that, the number 10**17 itself also has a Zeckendorf's representation.
      So, let 10**17 = f(a_1) + f(a_2) + ... + f(a_m) for decreasing a_i as i increases
    . Let W[a,b] be the number of numbers in Zeckendorf's representation of numbers in the range [a, b].
      and W[a] be the number of numbers in Zeckendorf's representation of 'a'.
    . So, we want to calculate W[1, 10**17-1].
      We have that,
        W[1, 10**17-1] = W[1, f(a_1)-1] + W[f(a_1)] + W[f(a_1)+1, 10**17-1]
                       = DP[a_1 - 1]    +      1    + W[f(a_1)+1, 10**17-1]
                       = DP[a_1 - 1]    +      1    + W[f(a_1)+1, f(a_1) + f(a_2)-1] + W[f(a_2)] + W[f(a_1) + f(a_2) + 1, 10**17-1]
                       = DP[a_1 - 1]    +      1    + ( W[1, f(a_2)-1] + f(a_2) )    +    1      + W[f(a_1) + f(a_2) + 1, 10**17-1]
                                                        ^^^^^^^^^^^^^^^^^^^^^^^
                                                        for each of term k in [1, f(a_2)-1], we add f(a_1) to k, then the range becomes [f(a_1)+1, f(a_1) + f(a_2)-1]
                                                        also, there are f(a_2) terms in [1, f(a_2)-1]
                                                        -> so W[f(a_1)+1, f(a_1) + f(a_2)-1] = W[1, f(a_2)-1] + f(a_2)
                       = DP[a_1 - 1]    +      1    + ( DP[a_2 - 1] + f(a_2) )    +    1      + W[f(a_1) + f(a_2) + 1, 10**17-1]
                       = DP[a_1 - 1]    +      1    + ( DP[a_2 - 1] + f(a_2) )    +    1      + W[f(a_1) + f(a_2) + 1, f(a_1) + f(a_2) + f(a_3) - 1] + W[f(a_3)] + W[f(a_1) + f(a_2) + f(a_3) + 1, 10**17-1]
                       = DP[a_1 - 1]    +      1    + ( DP[a_2 - 1] + f(a_2) )    +    1      + (W[1, f(a_3) - 1] + 2 * f(a_3))                      +     1     + W[f(a_1) + f(a_2) + f(a_3) + 1, 10**17-1]
                                                                                                                   ^^^^^^^^^^^^^
                                                                                                                   now add 2 Fibonacci numbers (f(a_1) and f(a_2)) to each term in [1, f(a_3) - 1], and there are f(a_3) terms in the range [1, f(a_3) - 1]
                       = and so on
                                                        
"""
N = 10**17

def generate_fibonacci_seq(upperbound):
    fibonacci = [0, 1]
    while True:
        fibonacci.append(fibonacci[-1] + fibonacci[-2])
        if fibonacci[-1] > upperbound:
            break
    return fibonacci

fibonacci = generate_fibonacci_seq(N)

M = len(fibonacci)

f = lambda x: fibonacci[x]

if __name__ == "__main__":
    ans = 0

    DP = [0] * (M+1)

    DP[2] = 1 # 1 -> f(3)-1 ; 1 -> 1 : 1
    DP[3] = 2 # 1 -> f(4)-1 ; 1 -> 2 : 2
    DP[4] = 5 # 1 -> f(5)-1 ; 1 -> 4 : 5
    for i in range(5, M):
        DP[i] = DP[i-1] + f(i-1) + DP[i-2]

    n = N-1
    i = M-1
    n_fibonacci_numbers_to_add_per_term = 0
    while n > 0:
        while f(i) > n:
            i -= 1
        n -= f(i)

        ans += DP[i-1] + n_fibonacci_numbers_to_add_per_term * f(i) + 1

        n_fibonacci_numbers_to_add_per_term += 1

    print(ans)
