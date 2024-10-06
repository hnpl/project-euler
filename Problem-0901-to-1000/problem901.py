# Question: https://projecteuler.net/problem=901

# . The expected time of drilling is, cost = t[0] + sum_{i = 1 -> inf} (t[i] * exp(-t[i-1])) where t[i] is the depth of i^th drill, and t[i] < t[i+1].
#                                                                      ^^^^^ | ^^^^^^^^^^^^^^
#                                                        depth of i^th drill | probability of failing on the (i-1)^th drill
#   I tried modeling this problem on SCIP but the it took way too long to converge to the solution. So we need more constraints.
#
# . Taking the derivative of cost over t[i], we have that,
#   d(cost) / dt[i] = d( ... + t[i] * exp(-t[i-1]) + t[i+1] * exp(-t[i])) / dt[i]
#                   = exp(-t[i-1]) - t[i+1] * exp(-t[i])
#   For the cost function to be optimal, d(cost) / dt[i] = 0
#               ->  exp(-t[i-1]) - t[i+1] * exp(-t[i]) = 0
#               ->                              t[i+1] = exp(-t[i-1]) / exp(-t[i])
#               ->                              t[i+1] = exp(t[i] - t[i-1]) (*)
#   Replace the (*) expression to the partial cost function, we have that,
#      cost = t[0] + sum_{i = 1 -> N-1} (t[i] * exp(-t[i-1]))
#           = t[0] + t[1] * exp(-t[0]) + t[2] * exp(-t[1]) + ... + t[N-1] * exp(-t[N-2])
#           = t[0] + t[1] * exp(-t[0]) + exp(t[1] - t[0]) * exp(-t[1]) + exp(t[2] - t[1]) * exp(-t[2]) + ... + exp(t[N-2] - t[N-3]) * exp(-t[N-2])
#           = t[0] + t[1] * exp(-t[0]) + exp(-t[0]) + exp(-t[1]) + ... + exp(-t[N-3])
#   So, to compute the cost function, we can start from a value of t[0], then use (*) to generate the rest of t[i].
#   Note that, the derivative shows us local optima, so the t[i] < t[i+1] might be violated. So, for every sequence of t that we check, we need to check for that constraint as well.
#   Doing a grid search shows that, for every t[0] such that 0.2057 < t[0] < 0.7465, the t[i] < t[i+1] constraint is violated.
#   Note that, for 0 < t[0] < 0.2057, the cost is monotonically decreasing as t[0] increases. So, the local optimal cost corresponds to the largest value of t[0] in the range that does not violate the t[i] < t[i+1] constraint.
#   Similarly, for 0.7465 < t[0] < 1, the cost is monotonically increasing as t[0] increases. So, the local optimal cost corresponds to the smallest value of t[0] in the range that does not violate the constraint.
#   So, we can do a binary search on (0, 0.2057) where the left side is expected to follow the t[i] < t[i+1] constraint while the right side violates the constraint.
#   Similarly, we can do a binary search on (0.7465, inf) where the left side is expected to violates the constraint while the right side is expected to follow the constraint.
#   Now, we have two results. We'll pick one with a smaller cost.

import numpy as np

def cost(t_0, max_n):
    is_t_monotonically_increasing = True
    t_1 = np.exp(t_0)
    t = [t_0, t_1]
    for i in range(2, max_n):
        if t[-1] - t[-2] > 500: # since we are doing e^(t[-1] - t[-2]), we constaint t[-1] - t[-2] to be smaller than 500 to avoid overflow
            break
        t.append(np.exp(t[-1] - t[-2]))
        if (t[-1] < t[-2]):
            is_t_monotonically_increasing = False
    t = np.array(t[:-2], dtype=np.float64)
    ans = t[0] + t[1] * np.exp(-t[0]) + np.sum(np.exp(-np.array(t[:-2], dtype=np.float64)))
    return ans, is_t_monotonically_increasing

def binary_search_1(left, right):
    l = left
    r = right
    prev_m = 0
    while True:
        m = (l + r) / 2
        if m == prev_m:
            break
        prev_m = m
        cost_value, is_t_monotonically_increasing = cost(m, 30)
        #print(m, l, r, is_t_monotonically_increasing)
        if is_t_monotonically_increasing:
            l = m
        else:
            r = m
    cost_value, _ = cost(l, 30)
    return cost_value

def binary_search_2(left, right):
    l = left
    r = right
    prev_m = 0
    while True:
        m = (l + r) / 2
        if m == prev_m:
            break
        prev_m = m
        cost_value, is_t_monotonically_increasing = cost(m, 30)
        #print(m, l, r, is_t_monotonically_increasing)
        if is_t_monotonically_increasing:
            r = m
        else:
            l = m
    cost_value, _ = cost(r, 30)
    return cost_value

local_optimal_cost_1 = binary_search_1(0, 0.2057)
local_optimal_cost_2 = binary_search_2(0.7465, 1)
ans = min(local_optimal_cost_1, local_optimal_cost_2)
print(ans)

# The below are attempts to use SCIP to solve the problem.
# The cost function converges close to the solution quite quickly (up to the 6^th digit after the decimal place), but the optimizer fails to produce
# the optimal solution within 10 mins.
"""
from pyscipopt import Model, quicksum, exp

N = 30
#epsilon_6 = 1e-6
epsilon_8 = 1e-8

model = Model()
model.setMinimize()
t = []
for i in range(N):
    t.append(model.addVar(f"t_{i}", lb=epsilon_8))
Y = model.addVar("Y")
model.addCons(Y >= t[0] + t[1] * exp(-t[0]) + quicksum([exp(-t[i]) for i in range(N-2)]))
for i in range(N-1):
    model.addCons(t[i+1] - t[i] >= epsilon_8)
for i in range(N-2):
    model.addCons(t[i+2] == exp(t[i+1]-t[i]))
model.addCons(t[0] >= 0.74654201)
model.setObjective(Y)
model.optimize()
sol = model.getBestSol()
import math
for i in range(N):
    print(f"t_{i} = {sol[t[i]]} -> e^-d = {math.exp(-sol[t[i]])}")
print(f"Y = {sol[Y]}")
"""

"""
from pyscipopt import Model, quicksum, exp

N = 30
epsilon_6 = 1e-6
epsilon_8 = 1e-8

model = Model()
model.setMinimize()
t = []
for i in range(N):
    t.append(model.addVar(f"t_{i}", lb=epsilon_8, ub=30))
Y = model.addVar("Y")
model.addCons(Y >= t[0] + quicksum([(exp(-t[i-1]))*t[i] for i in range(1, N)]))
model.addCons(Y <= t[0] + quicksum([(exp(-t[i-1]))*t[i] for i in range(1, N)]) + epsilon_8)
for i in range(N-1):
    model.addCons(t[i+1] - t[i] >= epsilon_6)
model.setObjective(Y)
model.optimize()
sol = model.getBestSol()
import math
for i in range(N):
    print(f"t_{i} = {sol[t[i]]} -> e^-d = {math.exp(-sol[t[i]])}")
print(f"Y = {sol[Y]}")
"""
