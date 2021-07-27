# Question: https://projecteuler.net/problem=666

"""
    . Let P(k) be the probability that, starting from a single bacterium of type k, this bacterium and its derivatives will eventually no longer exist.
    . Then we will have to solve for the root of a system of equations.
    . For example, in S(2,2), the bacterium of type alpha will either clone itself or split into 3 beta, then we have
            P(alpha) = 1/2 * P(alpha) * P(alpha) + 1/2 * [P(beta) ** 3]
    . We will have a system of K equations and K variables, and we can find the root of this system using scipy.optimize.
    . Since the scenario starts with a bacterium of type a_0, the answer is P(a_0).
"""

import scipy.optimize

K = 500
M = 10

def test(x):
    new_y = [x[0] - 0.5 * (x[0] ** 2) - 0.5 * (x[1] ** 3),
             x[1] - 0.5 * x[0] * x[1] - 0.5]
    return new_y

def sequence_generator(n):
    seq = [306]
    while len(seq) < n:
        seq.append((seq[-1]**2) % 10007)
    return seq

def get_transformation_map(r):
    # type K: the bacterium cedes to exist
    T = {t: {} for t in range(K+1)}
    T[K][(K,)] = 1

    for curr_type in range(K):
        for j in range(M):
            q = r[curr_type * M + j] % 5
            if q == 0:
                if not (K,) in T[curr_type]:
                    T[curr_type][(K,)] = 0
                T[curr_type][(K,)] += 1/M                              # 1 curr_type will transfrom to 1 type K with the probability of 1/M
            if q == 1:
                if not (curr_type, curr_type) in T[curr_type]:
                    T[curr_type][(curr_type, curr_type)] = 0
                T[curr_type][(curr_type, curr_type)] += 1/M            # 1 curr_type will transform to 2 curr_type with the probability of 1/M
            if q == 2:
                new_type = (2*curr_type) % K
                if not (new_type,) in T[curr_type]:
                    T[curr_type][(new_type,)] = 0
                T[curr_type][(new_type,)] += 1/M                       # 1 curr_type will transform to 1 (2*curr_type)%k with the probability of 1/M
            if q == 3:
                new_type = (curr_type**2+1)%K
                if not (new_type, new_type, new_type) in T[curr_type]:
                    T[curr_type][(new_type, new_type, new_type)] = 0
                T[curr_type][(new_type, new_type, new_type)] += 1/M    # 1 curr_type will transform to 3 (curr_type**2+1)%K with the probability of 1/M
            if q == 4:
                new_type = (curr_type+1)%K
                if not (curr_type, new_type) in T[curr_type]:
                    T[curr_type][(curr_type, new_type)] = 0
                T[curr_type][(curr_type, new_type)] += 1/M             # 1 curr_type will transform to 1 curr_type and 1 (curr_type+1)%K with the probability of 1/M
    return T

r = sequence_generator((M+2)* K)
T = get_transformation_map(r)

def f(x):
    new_y = [0] * K

    for curr_type in range(K):
        next_sum = x[curr_type]
        for next_types, probability in T[curr_type].items():
            prod = probability
            for next_type in next_types:
                if not next_type == K:
                    prod *= x[next_type]
            next_sum -= prod
        new_y[curr_type] = next_sum
    return new_y


if __name__ == "__main__":
    ans = 0

#    sol = scipy.optimize.root(test, [1, 0], method='broyden2', tol=1e-8)
#    ans = sol.x[0]

    initial_state = [0] * K
    initial_state[0] = 1
    sol = scipy.optimize.root(f, initial_state, method='hybr', tol=1e-12)
    ans = sol.x[0]
    print(sol)
 
    print(ans)
