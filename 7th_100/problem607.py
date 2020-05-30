# Question: https://projecteuler.net/problem=607

# TODO: explain this

import numpy as np
from scipy.optimize import minimize
from numpy import sqrt

sqrt2 = np.sqrt(2)

def objective(x):
    x1, x2, x3, x4, x5, x6 = x
    return 1/9*sqrt((x1 - x2 + 10*sqrt(2))**2 + (x1 - x2)**2) + 1/10*sqrt((x1 + 25*sqrt(2) - 50)**2 + x1**2) + 1/8*sqrt((x2 - x3 + 10*sqrt(2))**2 + (x2 - x3)**2) + 1/7*sqrt((x3 - x4 + 10*sqrt(2))**2 + (x3 - x4)**2) + 1/6*sqrt((x4 - x5 + 10*sqrt(2))**2 + (x4 - x5)**2) + 1/5*sqrt((x5 - x6 + 10*sqrt(2))**2 + (x5 - x6)**2) + 1/10*sqrt((x6 - 25*sqrt(2) - 50)**2 + (x6 - 100)**2)

x0 = np.array([0.0, 0.0, 0.0, 0.0, 0.0, 0.0])
res = minimize(objective, x0, method='Nelder-Mead', options={'xatol':1e-10, 'fatol':1e-10, 'disp': True, 'maxiter': 20000})
ans = objective(res.x)
print("{0:.10f}".format(ans))
