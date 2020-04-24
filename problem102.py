# https://projecteuler.net/problem=102

# https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
# https://en.wikipedia.org/wiki/Barycentric_coordinate_system

class Point2D:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y
    def add(self, x2, y2):
        return Point2D(self.x + x2, self.y + y2)
    def subtract(self, x2, y2):
        return Point2D(self.x - x2, self.y - y2)
    def scalar_mul(self, l):
        return Point2D(l * self.x, l * self.y)

class Triangle:
    def __init__(self, p1, p2, p3):
        self.p1 = p1
        self.p2 = p2
        self.p3 = p3
    # Shoelace formula for triangles
    # https://en.wikipedia.org/wiki/Shoelace_formula
    def area(self):
        return 0.5 * ((self.p1.x - self.p3.x)*(self.p2.y-self.p1.y)-(self.p1.x-self.p2.x)*(self.p3.y-self.p1.y))
    # https://stackoverflow.com/a/14382692
    # triangle (p0, p1, p2)
    # testing point p
    # s = 1/(2*Area)*(p0y*p2x - p0x*p2y + (p2y - p0y)*px + (p0x - p2x)*py);
    # t = 1/(2*Area)*(p0x*p1y - p0y*p1x + (p0y - p1y)*px + (p1x - p0x)*py);
    # (2*Area) = s * (...)
    # (2*Area) = t * (...)
    # point p is inside the triangle if s > 0 and t > 0 and 1 - s - t > 0
    def point_inside_triangle(self, p):
        area = self.area()
        sign = -1 if area < 0 else 1
        area = area * sign
        p1, p2, p3 = self.p1, self.p2, self.p3
        s = p1.y * p3.x - p1.x * p3.y + (p3.y - p1.y) * p.x + (p1.x - p3.x) * p.y
        s *= sign
        t = p1.x * p2.y - p1.y * p2.x + (p1.y - p2.y) * p.x + (p2.x - p1.x) * p.y
        t *= sign
        return s > 0 and t > 0 and s + t < 2 * area

    def has_origin_inside_triangle(self):
        area = self.area()
        sign = -1 if area < 0 else 1
        area = area * sign
        p1, p2, p3 = self.p1, self.p2, self.p3
        s = p1.y * p3.x - p1.x * p3.y
        s *= sign
        t = p1.x * p2.y - p1.y * p2.x
        t *= sign
        return s > 0 and t > 0 and (s + t) < 2 * area

triangles = []

with open('inputs/p102_triangles.txt', 'r') as f:
    for line in f:
        p = map(float, line.strip().split(','))
        t = Triangle(
            Point2D(next(p), next(p)),
            Point2D(next(p), next(p)),
            Point2D(next(p), next(p))
        )
        triangles.append(t)

count = 0
for triangle in triangles:
    if triangle.has_origin_inside_triangle():
        count = count + 1

print(count)

