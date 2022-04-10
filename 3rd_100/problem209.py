# Question:https://projecteuler.net/problem=209

"""
    * For each value of (a,b,c,d,e,f), we need to assign Tau(a,b,c,d,e,f) to either 0 or 1.
    * Per a truth table, there are 64 tuples of (a,b,c,d,e,f) -> there are 2^64 ways of constructing a truth table.
    * While assigning value to Tau(a,b,c,d,e,f), there is only one constraint,
        Tau(a,b,c,d,e,f) AND Tau(b,c,d,e,f,a XOR (b AND c)) = 0
      Meaning, for each truth table, we should assign values for Tau such that, Tau(a,b,c,d,e,f) and Tau(b,c,d,e,f,a XOR (b AND c)) cannot not be 1 at the same time for all tuples (a,b,c,d,e,f).
    * Observation:
        * Starting from a tuple (a,b,c,d,e,f), we can construct a sequence of tuples such that,
            next((a,b,c,d,e,f)) = (b, c, d, e, f, a XOR (b AND c))
        * The above sequence is cyclic.
        * The cyclic sequence are,
        ** [In the binary representation]
        0b000000 -> 
        0b000001 -> 0b000010 -> 0b000100 -> 0b001000 -> 0b010000 -> 0b100000 -> 
        0b000011 -> 0b000110 -> 0b001100 -> 0b011000 -> 0b110001 -> 0b100011 -> 0b000111 -> 0b001110 -> 0b011100 -> 0b111001 -> 0b110010 -> 0b100101 -> 0b001011 -> 0b010110 -> 0b101100 -> 0b011001 -> 0b110011 -> 0b100111 -> 0b001111 -> 0b011110 -> 0b111101 -> 0b111010 -> 0b110100 -> 0b101001 -> 0b010011 -> 0b100110 -> 0b001101 -> 0b011010 -> 0b110101 -> 0b101011 -> 0b010111 -> 0b101110 -> 0b011101 -> 0b111011 -> 0b110110 -> 0b101101 -> 0b011011 -> 0b110111 -> 0b101111 -> 0b011111 -> 0b111111 -> 0b111110 -> 0b111100 -> 0b111000 -> 0b110000 -> 0b100001 -> 
        0b000101 -> 0b001010 -> 0b010100 -> 0b101000 -> 0b010001 -> 0b100010 -> 
        0b001001 -> 0b010010 -> 0b100100 -> 
        0b010101 -> 0b101010 ->
        ** [In the decimal representation]
        [0]
        [1, 2, 4, 8, 16, 32]
        [3, 6, 12, 24, 49, 35, 7, 14, 28, 57, 50, 37, 11, 22, 44, 25, 51, 39, 15, 30, 61, 58, 52, 41, 19, 38, 13, 26, 53, 43, 23, 46, 29, 59, 54, 45, 27, 55, 47, 31, 63, 62, 60, 56, 48, 33]
        [5, 10, 20, 40, 17, 34]
        [9, 18, 36]
        [21, 42]
    * So, as long that there are no adjacent elements, say, alpha and beta, in the cycle having Tau(alpha) = Tau(beta) = 1, then we can construct each cycle.
    Meaning, for each cycle, we need to count the number of way of assigning Tau to all elements in the cycle such that there are no adjacent elements having Taus being 1 at the same time.
    Note that, since the sequence is cyclic, the tau of the last element of the "sequence" and the tau of the first element of the sequence should not be 1 at the same time.
        - We can do this with dynamic programming (or Markov chain):
            Let a state being (cycle_length, tau_of_the_first_element, tau_of_thelast_element)
            Then, DP[cycle_length, 0, 0] = DP[cycle_length-1, 0, 0] + DP[cycle_length-1, 0, 1] // Adding 0 to sequences that were previously ended in 0 or 1.
                  DP[cycle_length, 0, 1] = DP[cycle_length-1, 0, 0] // Adding 1 to sequence that were previously ended in 0.
                  DP[cycle_length, 1, 0] = DP[cycle_length-1, 1, 0] + DP[cycle_length-1, 1, 1] // Adding 0 to sequences that were previously ended in 0 or 1.
                  DP[cycle_length, 1, 1] = DP[cycle_length-1, 1, 0] // Adding 1 to sequence that were previously ended in 0.
"""

N = 6

def int_to_bin(n, n_bits):
    b = list(map(int, bin(n)[2:].zfill(n_bits)))
    return b

def bin_to_int(bits):
    return int("".join(map(str, bits)), 2)

def dfs(bits, seen):
    next_bits = bits[1:]
    next_bits.append(bits[0] ^ (bits[1] and bits[2]))
    next_n = bin_to_int(next_bits)
    if not next_n in seen:
        seen.append(next_n)
        dfs(next_bits, seen)

def get_DP(length):
    # DP[i][j][k] -> number of strings of length of i such that the strings start with j and end with k, and the strings do not have adjacent 1's
    DP = [[[0 for k in range(2)] for j in range(2)] for i in range(length+1)]

    DP[1][0][0] = 1
    DP[1][1][1] = 1

    for length in range(2, length+1):
        for starting_char in [0, 1]:
            for ending_char in [0, 1]:
                prev_chars = [0] if ending_char == 1 else [0, 1]
                for prev_char in prev_chars:
                    DP[length][starting_char][ending_char] += DP[length-1][starting_char][prev_char]
    return DP

if __name__ == "__main__":

    visited = set()
    cycle_lengths = []
    for n in range(2**N):
        if n in visited:
            continue
        bits = int_to_bin(n, N)
        seen = [n]
        dfs(bits, seen)
        visited |= set(seen)
        cycle_lengths.append(len(seen))
        print(seen)
    DP = get_DP(max(cycle_lengths))

    ans = 1

    for length in cycle_lengths:
        ans *= DP[length][0][0] + DP[length][0][1] + DP[length][1][0]

    print(ans)
