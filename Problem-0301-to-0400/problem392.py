import scipy as sp
import scipy.optimize as optimize
from scipy.optimize import Bounds, LinearConstraint
from math import sqrt, pi, sin, cos
import numpy as np

N = 400

def S(p):
    val = 0
    n = len(p)
    a = [0.0] + list(p)
    """
    for k in range(1, n+1):
        val += (1-sin(a[k])) * (cos(a[k-1]) - cos(a[k]))
    return -val
    """
    val = - cos(a[n]) + 1.0
    for k in range(1, n+1):
        val -= sin(a[k]) * (cos(a[k-1]) - cos(a[k]))
    return -val

def G(p):
    g = []
    a = [0.0] + list(p)
    for k in range(1, len(p)):
        g.append(cos(2*a[k]) - cos(a[k]) * cos(a[k-1]) - sin(a[k+1]*sin(a[k])))
    g.append(sin(a[-1]) + cos(2*a[-1]) - cos(a[-1]) * cos(a[-2]))
    for i in range(len(g)):
        g[i] = -g[i]
    return g

if __name__ == "__main__":
    initial_x = []
    for i in range(N//2):
        #initial_x.append(1-(i+1)/(N+1))
        initial_x.append(0)
    bounds = Bounds([0 for i in range(N//2)], [pi/2 for i in range(N//2)])

    M = np.zeros((N//2, N//2), dtype=np.float64)
    for i in range(0, N//2-1):
        M[i][i] = 1.0
        M[i][i+1] = -1.0

    linear_constraints = LinearConstraint(M, [-np.inf for i in range(N//2)], [0 for i in range(N//2-1)]+[0])

    tol = 1e-12

    result = optimize.minimize(S, initial_x, 
                               #jac=G,
                               jac='3-point',
                               bounds=bounds,
                               constraints = linear_constraints,
                               method = "trust-constr",
                               tol = tol,
                               options={'xtol': tol, 'gtol': tol, 'barrier_tol': tol, 'maxiter': 2000,'verbose':1})
    """

    constraints = []
    for i in range(0, N//2):
        lowerbound = {'type': 'ineq', 'fun': lambda x, index = i: x[index]}
        upperbound = {'type': 'ineq', 'fun': lambda x, index = i: pi/2 - x[index]}
        constraints.append(lowerbound)
        constraints.append(upperbound)
        if not i == 0:
            increasing = {'type': 'ineq', 'fun': lambda x, index = i: x[index] - x[index-1]}
            constraints.append(increasing)

    result = optimize.minimize(S, initial_x,
                               constraints = constraints,
                               method = "COBYLA",
                               tol = tol,
                               options={'maxiter': 10000})
    """
    print(result)
    print("ans:", 4 + result.fun*4)
