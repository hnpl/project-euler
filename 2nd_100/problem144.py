# Question: https://projecteuler.net/problem=144

import math
import matplotlib.pyplot as plt 
from matplotlib.patches import Ellipse

x_i, y_i = 1.4, -9.6
count = 1
x, y = x_i, y_i
prev_x, prev_y = 0, 10.1

"""
ellipse = Ellipse(xy = (0,0), width = 2*5, height = 2*10, edgecolor='black', lw = 1, facecolor='none')
fig = plt.figure(0)
ax = fig.add_subplot(111, aspect='equal')
ax.add_artist(ellipse)
plt.plot([x, prev_x], [y, prev_y], marker = 'o')
"""

while (not (x <= 0.01 and x >= -0.01 and y > 0)):
    # y = mx + b
    # b = y - mx
    # tangent line
    m = (-4*x/y)
    # angle of the tangent line
    am = math.atan2(y, 4*x)
    # angle of the incoming beam
    an = math.atan2((y-prev_y), (x-prev_x))
    an = math.pi + an

    am = 2*(math.pi/2 - (an - am)) + an
    m = math.tan(am)
    b = y - m*x

    # 4x^2 + (mx + b)^2 = 100
    # (4+m^2)x^2 + 2mbx + (b^2 - 100) = 0
    delta = (m*b)**2 - (4+m**2) * (b**2 - 100)
    assert(delta > 0)
    sqrt_delta = math.sqrt(delta)
    x1 = (-m*b + sqrt_delta) / (4+m**2)
    x2 = (-m*b - sqrt_delta) / (4+m**2)
    prev_x, prev_y = x, y
    if abs(x - x1) < 0.0001:
        x = x2
    else:
        x = x1
    y = m * x + b
    count = count + 1
    
    """
    plt.plot([x, prev_x], [y, prev_y], marker = 'o')
    """

ans = count - 1
print(ans)

"""
ax.set_xlim(-15, 15)
ax.set_ylim(-15, 15)
plt.savefig('beams.png')
"""